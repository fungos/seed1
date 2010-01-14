/*! \file SdlImage.cpp
	\author	Danny Angelo Carminati Grein
	\brief Image SDL Implementation
*/


#ifdef _SDL_

#include "Image.h"
#include "FileSystem.h"
#include "MemoryManager.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"

#define TAG "[Image] "


namespace Seed { namespace SDL {


const char *const pImageFormatTable[] = {"TGA", "PNG", "JPG"};
enum eImageFormat
{
	TGA,
	PNG,
	JPG
};

IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Image *image = new Image();
	image->Load(filename, res, pool);

	return image;
}

Image::Image()
	: pSurface(NULL)
#if defined(DEBUG)
	, stFile()
#endif
	, iTextureId(0)
	, fWidth(0.0f)
	, fHeight(0.0f)
{
}

Image::~Image()
{
	this->Reset();
}

INLINE void Image::Reset()
{
	this->UnloadTexture();

	if (pSurface)
		SDL_FreeSurface(pSurface);

	this->pSurface = NULL;
	this->pPool = NULL;

	this->fWidth = 0;
	this->fHeight = 0;

#if defined(DEBUG)
	stFile.Close();
#endif
}

BOOL Image::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		this->pPool = pool;
		this->pRes = res;
		this->pFilename = filename;

	#if defined(DEBUG)
		pFileSystem->Open(filename, &stFile, pool);
	#else
		File stFile(filename, pool);
	#endif

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
				Info(TAG "Make sure that libpng12-0.dll and zlib1.dll is in the exact same folder than this application.");
			
			ASSERT(false);
		}

		if (tmp->format->BitsPerPixel != 32)
		{
			SDL_SetAlpha(tmp, 0, SDL_ALPHA_OPAQUE);
		}

		pSurface = SDL_DisplayFormatAlpha(tmp);
		ASSERT_NULL(pSurface);
		SDL_FreeSurface(tmp);

		//Log(TAG "%dx%d", pSurface->w, pSurface->h);
		this->fWidth = (f32)pSurface->w / (f32)pScreen->GetWidth();
		this->fHeight = (f32)pSurface->h / (f32)pScreen->GetHeight();

	#if SEED_ENABLE_PRELOAD_TEXTURE == 1
	//	this->LoadTexture();
	#endif // SEED_ENABLE_PRELOAD_TEXTURE

	#if SEED_ENABLE_KEEP_IMAGE_DATA != 1
		stFile.Close();
	#endif

		this->bLoaded = TRUE;
	}

	return this->bLoaded;
}

/*
BOOL Image::Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool)
{
	ASSERT_MSG(FALSE, "Not supported!");
}
*/

BOOL Image::Unload()
{
	this->UnloadTexture();

	if (pSurface)
		SDL_FreeSurface(pSurface);

	pSurface = NULL;
	this->bLoaded = FALSE;

	return TRUE;
}

INLINE const void *Image::GetData() const
{
	if (!pSurface)
		return NULL;

	return this->pSurface->pixels;
}

INLINE void Image::PutPixel(u32 x, u32 y, PIXEL px)
{
	if (!pSurface)
		return;

	u32 i_bpp = pSurface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pSurface->pixels + y * pSurface->pitch + x * i_bpp;

	switch (i_bpp)
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

INLINE PIXEL Image::GetPixel(u32 x, u32 y) const
{
	if (!pSurface)
		return 0;

	u32 i_bpp = pSurface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pSurface->pixels + y * pSurface->pitch + x * i_bpp;

	switch (i_bpp)
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

INLINE u8 Image::GetPixelAlpha(u32 x, u32 y) const
{
	if (!pSurface)
		return 0;

	if (x > (u32)pSurface->w || y > (u32)pSurface->h)
	{
		#if !defined(__GNUC__)
			__asm { int 3 };
		#else
			asm("int $3");
		#endif
		Log(TAG "Pixel position out of bounding!");
		return 255;
	}

	u8 a = 255;
	u8 r, g, b;
	if (pSurface->format->BitsPerPixel == 32)
	{
		PIXEL px = this->GetPixel(x, y);
		SDL_GetRGBA(px, pSurface->format, &r, &g, &b, &a);
	}

	return a;
}

INLINE u32 Image::GetWidthInPixel() const
{
	if (!pSurface)
		return 0;

	return pSurface->w;
}

INLINE u32 Image::GetHeightInPixel() const
{
	if (!pSurface)
		return 0;

	return pSurface->h;
}

INLINE f32 Image::GetWidth() const
{
	return this->fWidth;
}

INLINE f32 Image::GetHeight() const
{
	return this->fHeight;
}

INLINE u32 Image::GetUsedMemory() const
{
	u32 size = IResource::GetUsedMemory() + sizeof(this);
	if (pSurface)
		size += (pSurface->h * pSurface->w * pSurface->format->BytesPerPixel);

	return size;
}

INLINE int Image::LoadTexture()
{
	if (pSurface && !iTextureId)
	{
		glGenTextures(1, &iTextureId);
		glBindTexture(GL_TEXTURE_2D, iTextureId);

		if (nMinFilter == Seed::TextureFilterLinear)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (nMinFilter == Seed::TextureFilterNearest)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (nMagFilter == Seed::TextureFilterLinear)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (nMagFilter == Seed::TextureFilterNearest)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		switch (pSurface->format->BitsPerPixel)
		{
			case 32:
				// OpenGL 1.2+ only GL_EXT_bgra
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pSurface->pixels);
			break;

			case 24:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSurface->w, pSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, pSurface->pixels);
			break;

			case 16:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSurface->w, pSurface->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pSurface->pixels);
			break;

			case 8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, pSurface->w, pSurface->h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pSurface->pixels);
			break;

			default:
			break;
		}
	}

	return this->iTextureId;
}

INLINE void Image::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin && filter == nMinFilter)
		return;

	if (type == Seed::TextureFilterTypeMag && filter == nMagFilter)
		return;

	if (iTextureId)
	{
		GLint old = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &old);
		glBindTexture(GL_TEXTURE_2D, iTextureId);

		if (type == Seed::TextureFilterTypeMin)
		{
			if (filter == Seed::TextureFilterLinear)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			if (filter == Seed::TextureFilterNearest)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else if (type == Seed::TextureFilterTypeMag)
		{
			if (filter == Seed::TextureFilterLinear)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			if (filter == Seed::TextureFilterNearest)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		glBindTexture(GL_TEXTURE_2D, old);
	}

	if (type == Seed::TextureFilterTypeMin)
	{
		nMinFilter = filter;
	}
	else if (type == Seed::TextureFilterTypeMag)
	{
		nMagFilter = filter;
	}
}

INLINE int Image::GetTexture()
{
	return this->iTextureId;
}

INLINE void Image::UnloadTexture()
{
	if (iTextureId)
		glDeleteTextures(1, &iTextureId);

	iTextureId = 0;
}


}} // namespace


#endif // _SDL_
