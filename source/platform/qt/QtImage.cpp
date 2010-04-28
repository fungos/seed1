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

/*! \file QtImage.cpp
	\author	Danny Angelo Carminati Grein
	\brief Image QT Implementation
*/

#if defined(_QT_)

#include "Image.h"
#include "FileSystem.h"
#include "MemoryManager.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"

#define TAG "[Image] "

namespace Seed { namespace QT {

IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);

	Image *image = new Image();
	image->Load(filename, res, pool);

	return image;
}

Image::Image()
	: pPool(NULL)
	, iTextureId(0)
	, iHalfWidth(0)
	, iHalfHeight(0)
{
}

Image::~Image()
{
	this->Unload();
}

INLINE BOOL Image::Unload()
{
	return this->Reset();
}

BOOL Image::Reset()
{
	this->UnloadTexture();

	pPool = NULL;

	fWidth = 0;
	fHeight = 0;

	iWidth = 0;
	iHeight = 0;
	iHalfWidth = 0;
	iHalfHeight = 0;

	return TRUE;
}

BOOL Image::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);
	UNUSED(pool);
	ASSERT_NULL(res);
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);

	if (this->Unload())
	{
		image = QImage(filename);

		if (image.isNull())
		{
			return FALSE;
		}

		image = image.convertToFormat(QImage::Format_ARGB32);

		iWidth = image.width();
		iHeight = image.height();

		iHalfWidth = iWidth >> 1;
		iHalfHeight = iHeight >> 1;

		fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
		fHeight = (f32)iHeight / (f32)pScreen->GetHeight();

#if SEED_ENABLE_PRELOAD_TEXTURE == 1
		//this->LoadTexture();
#endif // SEED_ENABLE_PRELOAD_TEXTURE
	}

	return TRUE;
}

BOOL Image::Load(u32 width, u32 height, PIXEL *buffer, IMemoryPool *pool)
{
	UNUSED(pool);
	ASSERT_NULL(pool);

	ASSERT_MSG((void *)ROUND_UP(buffer, 32) == (void *)buffer, "ERROR: User image buffer MUST BE 32bits aligned!");
	ASSERT_MSG(ROUND_UP(width, 32) == width, "ERROR: User image scanline MUST BE 32bits aligned - pitch/stride!");

	if (this->Unload())
	{
		image = QImage((uchar *)buffer, width, height, QImage::Format_ARGB32);

		if (image.isNull())
		{
			return FALSE;
		}

		iWidth = width;
		iHeight = height;

		iHalfWidth = iWidth >> 1;
		iHalfHeight = iHeight >> 1;

		fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
		fHeight = (f32)iHeight / (f32)pScreen->GetHeight();

#if SEED_ENABLE_PRELOAD_TEXTURE == 1
		//this->LoadTexture();
#endif // SEED_ENABLE_PRELOAD_TEXTURE
	}

	return TRUE;
}

INLINE const void *Image::GetData() const
{
	return image.bits();
}

INLINE void Image::PutPixel(u32 x, u32 y, PIXEL px)
{
	u8 mR = PIXEL_GET_R(px);
	u8 mG = PIXEL_GET_G(px);
	u8 mB = PIXEL_GET_B(px);
	u8 mA = PIXEL_GET_A(px);

	image.setPixel(x, y, qRgba(mR, mG, mB, mA));
}

INLINE PIXEL Image::GetPixel(u32 x, u32 y) const
{
	QRgb px = image.pixel(x, y);
	return PIXEL_COLOR(qRed(px), qGreen(px), qBlue(px), qAlpha(px));
}

INLINE u8 Image::GetPixelAlpha(u32 x, u32 y) const
{
	QRgb px = image.pixel(x, y);
	return (u8)(qAlpha(px));
}

INLINE u32 Image::GetUsedMemory() const
{
	return image.byteCount() + sizeof(this);
}

INLINE int Image::LoadTexture()
{
	bool a = !image.isNull();
	bool b = (iTextureId == 0);
	if (a && b)
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

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Works on x86, so probably works on all little-endian systems.
		// Does it work on big-endian systems?
		glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return this->iTextureId;
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

#endif // _QT_
