/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file SdlTexture.cpp
	\author	Danny Angelo Carminati Grein
	\brief Texture SDL Implementation
*/

#if defined(_SDL_)

#include "Texture.h"
#include "FileSystem.h"
#include "MemoryManager.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"
#include "RendererDevice.h"

#define TAG "[Texture] "

namespace Seed { namespace SDL {

const char *const pImageFormatTable[] = {"TGA", "PNG", "JPG"};
enum eImageFormat
{
	TGA,
	PNG,
	JPG
};

IResource *TextureResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Texture *image = New(Texture());
	image->Load(filename, res, pool);

	return image;
}

Texture::Texture()
	: pSurface(NULL)
	, pData(NULL)
	, pTextureId(NULL)
	, iBytesPerPixel(0)
	, iPitch(0)
	, iAtlasWidth(0)
	, iAtlasHeight(0)
{
}

Texture::~Texture()
{
	this->Reset();
}

INLINE void Texture::Reset()
{
	ITexture::Reset();

	this->UnloadTexture();

	if (pSurface)
		SDL_FreeSurface(pSurface);
	pSurface = NULL;
	pData = NULL;
	pPool = NULL;

	iWidth = 0;
	iHeight = 0;
	iBytesPerPixel = 0;
	iPitch = 0;
	iAtlasWidth = 0;
	iAtlasHeight = 0;

	fWidth = 0.0f;
	fHeight = 0.0f;
}

BOOL Texture::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	if (ITexture::Load(filename, res, pool))
	{
		SDL_RWops *rwops = SDL_RWFromConstMem(stFile.GetData(), stFile.GetSize());

		SIZE_T extpos = STRLEN(stFile.GetName());
		char *ext = const_cast<char *>(stFile.GetName()) - 3;
		ext = &ext[extpos];

		u32 format = PNG;
		if (!STRCASECMP(pImageFormatTable[TGA], ext))
			format = TGA;
		else if (!STRCASECMP(pImageFormatTable[JPG], ext))
			format = JPG;

		SDL_Surface *tmp = IMG_LoadTyped_RW(rwops, 1, const_cast<char *>(pImageFormatTable[format]));

		if (!tmp)
		{
			Info(TAG "IMG_Load_RW ERROR: %s\n", IMG_GetError());

			if (format == PNG)
				Info(TAG "Make sure that libpng12-0.dll and zlib1.dll are in the exact same folder than this application binary.");

			ASSERT(false);
		}

		if (tmp->format->BitsPerPixel != 32)
		{
			SDL_SetAlpha(tmp, 0, SDL_ALPHA_OPAQUE);
		}

		pSurface = SDL_DisplayFormatAlpha(tmp);
		ASSERT_NULL(pSurface);
		SDL_FreeSurface(tmp);

		iWidth = pSurface->w;
		iHeight = pSurface->h;

		/*
		If the image isn't power of two, we need fix it.
		*/
		u32 width = 1;
		while (width < iWidth)
			width *= 2;

		u32 height = 1;
		while (height < iHeight)
			height *= 2;

		eRendererDeviceType type = pConfiguration->GetRendererDeviceType();
		if (type >= Seed::RendererDeviceOpenGLES && type <= Seed::RendererDeviceOpenGL40)
		{
			/*
			HACK:
			When using OpenGL we must pass power of 2 textures to the device, so we check if the texture need fix and create
			a new and correct surface for it. When using DirectX the device will not use SDL Surface to load texture, it will
			load directly from our in memory file and then close it, so we don't need to fix our texture.
			*/

			if (width != iWidth || height != iHeight)
			{
				Log(TAG "WARNING: texture size not optimal, changing from %dx%d to %dx%d", iWidth, iHeight, width, height);

				SDL_Surface *pTempSurface = NULL;
				Uint32 rmask, gmask, bmask, amask;

				#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					rmask = 0xff000000;
					gmask = 0x00ff0000;
					bmask = 0x0000ff00;
					amask = 0x000000ff;
				#else
					rmask = 0x000000ff;
					gmask = 0x0000ff00;
					bmask = 0x00ff0000;
					amask = 0xff000000;
				#endif

				pTempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA , width, height, 32, bmask, gmask, rmask, amask);

				SDL_SetAlpha(pTempSurface, 0, SDL_ALPHA_OPAQUE);
				SDL_SetAlpha(pSurface, 0, SDL_ALPHA_OPAQUE);
				SDL_BlitSurface(pSurface, NULL, pTempSurface, NULL);
				SDL_SetAlpha(pTempSurface, 0, SDL_ALPHA_TRANSPARENT);
				SDL_SetAlpha(pSurface, 0, SDL_ALPHA_TRANSPARENT);

				SDL_FreeSurface(pSurface);
				pSurface = pTempSurface;
			}
		}

		fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
		fHeight = (f32)iHeight / (f32)pScreen->GetHeight();
		iAtlasWidth = pSurface->w;
		iAtlasHeight = pSurface->h;

		// Lets keep the iWidth and iHeight the original one so the sprite rect can match it.
		// For texture UV mapping, we use the relation between original W and H and the converted texture W and H.
		//iWidth = pSurface->w;
		//iHeight = pSurface->h;

		iBytesPerPixel = pSurface->format->BytesPerPixel;
		iPitch = pSurface->pitch;
		pData = pSurface->pixels;

		pRendererDevice->TextureRequest(this, &pTextureId);

		bLoaded = TRUE;
	}
	else
	{
		Log(TAG "ERROR: Could not find/load texture %s.", filename);
	}

	return bLoaded;
}

BOOL Texture::Load(u32 width, u32 height, PIXEL *buffer, u32 atlasWidth, u32 atlasHeight, IMemoryPool *pool)
{
	ASSERT_NULL(buffer);
	//ASSERT_NULL(pool);

	ASSERT_MSG(ALIGN_FLOOR(buffer, 32) == (u8 *)buffer, "ERROR: User texture buffer MUST BE 32bits aligned!");
	ASSERT_MSG(ROUND_UP(width, 32) == width, "ERROR: User texture scanline MUST BE 32bits aligned - pitch/stride!");

	if (this->Unload())
	{
		pPool = pool;
		pFilename = "[dynamic sdl texture]";

		fWidth = (f32)width / (f32)pScreen->GetWidth();
		fHeight = (f32)height / (f32)pScreen->GetHeight();
		iWidth = iAtlasWidth = width;
		iHeight = iAtlasHeight = height;

		if (atlasWidth)
			iAtlasWidth = atlasWidth;

		if (atlasHeight)
			iAtlasHeight = atlasHeight;

		iBytesPerPixel = 4; // FIXME: parametized?
		iPitch = ROUND_UP(width, 32); // FIXME: parametized?
		pData = buffer;

		pRendererDevice->TextureRequest(this, &pTextureId);

		bLoaded = TRUE;
	}

	return bLoaded;
}

void Texture::Update(PIXEL *data)
{
	//this->UnloadTexture();
	//pRendererDevice->TextureRequest(this, &pTextureId);
	pData = data;
	pRendererDevice->TextureDataUpdate(this);
}

BOOL Texture::Unload()
{
	this->UnloadTexture();

	if (pSurface)
		SDL_FreeSurface(pSurface);

	pSurface = NULL;
	bLoaded = FALSE;

	return TRUE;
}

INLINE const void *Texture::GetData() const
{
	return pData;
}

INLINE u32 Texture::GetAtlasWidthInPixel() const
{
	return iAtlasWidth;
}

INLINE u32 Texture::GetAtlasHeightInPixel() const
{
	return iAtlasHeight;
}

INLINE void Texture::PutPixel(u32 x, u32 y, PIXEL px)
{
	if (!pSurface)
		return;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 3:
		{
			//u8 a = (px & LIB_PIXEL_A_MASK) >> LIB_PIXEL_A_SHIFT;
			u8 r = static_cast<u8>((px & LIB_PIXEL_R_MASK) >> LIB_PIXEL_R_SHIFT);
			u8 g = static_cast<u8>((px & LIB_PIXEL_G_MASK) >> LIB_PIXEL_G_SHIFT);
			u8 b = static_cast<u8>((px & LIB_PIXEL_B_MASK) >> LIB_PIXEL_B_SHIFT);

			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = r;
				p[1] = b;
				p[2] = g;
			}
			else
			{
				p[0] = g;
				p[1] = b;
				p[2] = r;
			}
		}
		break;

		case 4:
		{
			*(u32 *)p = px;
		}
		break;

		default:
		break;
	}
}

INLINE PIXEL Texture::GetPixel(u32 x, u32 y) const
{
	if (!pSurface)
		return 0;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 1:
			return *p;

		case 2: /* This will cause some problems ... */
			return *(u16 *)p;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(u32 *)p;

		default:
			return 0; /* shouldn't happen, but avoids warnings */
	}
}

INLINE u8 Texture::GetPixelAlpha(u32 x, u32 y) const
{
	if (!pSurface)
		return 0;

	if (x >= iWidth)
	{
		x = static_cast<u32>(pSurface->w) - 1;
		return this->GetPixelAlpha(x, y);
	}
	else if (y >= iHeight)
	{
		y = static_cast<u32>(pSurface->h) - 1;
		return this->GetPixelAlpha(x, y);
	}

	u8 a = 255;
	u8 r, g, b;
	if (iBytesPerPixel == 4)
	{
		PIXEL px = this->GetPixel(x, y);

		if (pSurface)
			SDL_GetRGBA(px, pSurface->format, &r, &g, &b, &a);
	}

	return a;
}

INLINE u32 Texture::GetUsedMemory() const
{
	return IResource::GetUsedMemory() + sizeof(this) + (iHeight * iWidth * iBytesPerPixel);
}

INLINE u32 Texture::GetBytesPerPixel() const
{
	return iBytesPerPixel;
}

INLINE void *Texture::GetTextureName() const
{
	return pTextureId;
}

INLINE void Texture::UnloadTexture()
{
	if (pTextureId)
	{
		pRendererDevice->TextureUnload(this);
	}
	else
	{
		pRendererDevice->TextureRequestAbort(this, &pTextureId);
	}

	pTextureId = NULL;
}

}} // namespace

#endif // _SDL_
