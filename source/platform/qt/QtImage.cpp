/*! \file QtImage.cpp
	\author	Danny Angelo Carminati Grein
	\brief Image QT Implementation
*/


#ifdef _QT_

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
	, pImage(NULL)
	, iTextureId(0)
	, fWidth(0.0f)
	, fHeight(0.0f)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
{
}

Image::~Image()
{
	this->Unload();
}

BOOL Image::Reset()
{
	this->UnloadTexture();
	//if (pImage)
	//	delete pImage;
	pImage = NULL;
	this->pPool = NULL;

	this->fWidth = 0;
	this->fHeight = 0;

	this->iWidth = 0;
	this->iHeight = 0;
	this->iHalfWidth = 0;
	this->iHalfHeight = 0;

	return TRUE;
}

BOOL Image::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);
	ASSERT_NULL(res);
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);

	if (this->Unload())
	{
		QImage img(filename);

		if (img.isNull())
		{
			return FALSE;
		}

		img = img.convertToFormat(QImage::Format_ARGB32);

		iWidth = img.width();
		iHeight = img.height();

		iHalfWidth = iWidth >> 1;
		iHalfHeight = iHeight >> 1;

		//img = img.scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		//Log(TAG "%dx%d", pSurface->w, pSurface->h);
		this->fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
		this->fHeight = (f32)iHeight / (f32)pScreen->GetHeight();

		pImage = &img;

#if SEED_ENABLE_PRELOAD_TEXTURE == 1
		this->LoadTexture();
#endif // SEED_ENABLE_PRELOAD_TEXTURE
	}

	return TRUE;
}

BOOL Image::Unload()
{
	return this->Reset();
}

INLINE const void *Image::GetData() const
{
	return pImage->bits();
}

INLINE void Image::PutPixel(u32 x, u32 y, PIXEL px)
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(px);
}

INLINE PIXEL Image::GetPixel(u32 x, u32 y) const
{
	UNUSED(x);
	UNUSED(y);
	return 0;
}

INLINE u8 Image::GetPixelAlpha(u32 x, u32 y) const
{
	u8 a = 255;
	UNUSED(x);
	UNUSED(y);

	return a;
}

INLINE u32 Image::GetWidthInPixel() const
{
	return iWidth;
}

INLINE u32 Image::GetHeightInPixel() const
{
	return iHeight;
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
	return pImage->byteCount() + sizeof(this);
}

INLINE int Image::LoadTexture()
{
	bool a = (pImage != NULL);
	bool b = !pImage->isNull();
	bool c = (iTextureId == 0);
	if (a && b && c)
	{
		glGenTextures(1, &iTextureId);
		glBindTexture(GL_TEXTURE_2D, iTextureId);

		// Works on x86, so probably works on all little-endian systems.
		// Does it work on big-endian systems?
		glTexImage2D(GL_TEXTURE_2D, 0, 4, pImage->width(), pImage->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, pImage->bits());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
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
