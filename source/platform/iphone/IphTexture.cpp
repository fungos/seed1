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

/*! \file IphTexture.cpp
	\author	Danny Angelo Carminati Grein
	\brief Texture Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Texture.h"
#include "FileSystem.h"
#include "MemoryManager.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"
#include "Formats.h"
#include "File.h"

#include <UIKit/UIKit.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <math.h>

#define TAG	"[Texture] "
#define MAX_TEXTURE_SIZE	1024

namespace Seed { namespace iPhone {

IResource *TextureResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);

	Texture *image = New(Texture());
	image->Load(filename, pool);

	return image;
}

Texture::Texture()
	: pImage(NULL)
	, stFile()
	, fWidth(0.0f)
	, fHeight(0.0f)
	, iTextureId(0)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, bCompressed(FALSE)
	, pixelFormat(kTexture2DPixelFormat_RGBA8888)
{
}

Texture::~Texture()
{
	this->Unload();
}

INLINE void Texture::Load(const char *filename, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);
	this->pPool = pool;

#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	this->LoadPNG(filename); // Ja configura W e H
#else
	this->LoadPVRTC(filename);
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

INLINE void Texture::Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool)
{
	ASSERT(width > 0);
	ASSERT(height > 0);
	ASSERT_NULL(buffer);
	ASSERT_NULL(pool);

	this->pPool = pool;
	this->iWidth = width;
	this->iHeight = height;
	this->pixelFormat = kTexture2DPixelFormat_RGBA8888;
	this->pImage = buffer;
}

INLINE void Texture::Unload()
{
	this->Reset();
}

INLINE void Texture::Reset()
{
	this->pImage = NULL;
	this->pPool = NULL;

	this->fWidth = 0;
	this->fHeight = 0;

	this->iWidth = 0;
	this->iHeight = 0;
	this->iHalfWidth = 0;
	this->iHalfHeight = 0;

	this->bCompressed = FALSE;
}

INLINE const void *Texture::GetData() const
{
	return pImage;
}

INLINE void Texture::PutPixel(u32 x, u32 y, PIXEL px)
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (this->pImage || pixelFormat != kTexture2DPixelFormat_RGB565 || pixelFormat != kTexture2DPixelFormat_A8)
	{
		const PIXEL *data1 = static_cast<const PIXEL *>(pImage);
		PIXEL *data = const_cast<PIXEL *>(data1);
		data[(y * this->iWidth) + x] = px; // ja deve ser arrumado em relacao ao atlas
	}
	else
	{
		Log(TAG "PutPixel: Format unsupported.");
	}
#else
	Log(TAG "PutPixel doesn't work with PVRTC!");
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

INLINE PIXEL Texture::GetPixel(u32 x, u32 y) const
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (!this->pImage)
		return 0;

	if (pixelFormat == kTexture2DPixelFormat_RGB565 || pixelFormat == kTexture2DPixelFormat_A8)
	{
		Log(TAG "GetPixel unsuported for format 565 (yet).");
		return 0;
	}

	const PIXEL *data = static_cast<const PIXEL *>(pImage);
	PIXEL px = data[(y * this->iWidth) + x]; // ja deve ser arrumado em relacao ao atlas

	return px;
#else
	return 0;
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

INLINE u8 Texture::GetPixelAlpha(u32 x, u32 y) const
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (!this->pImage)
		return 255;

	if (pixelFormat == kTexture2DPixelFormat_RGB565 || pixelFormat == kTexture2DPixelFormat_A8)
	{
		Log(TAG "GetPixel unsuported for format 565 (yet).");
		return 255;
	}

	PIXEL px = this->GetPixel(x, y);

	return ((PIXEL_A_MASK & px) >> PIXEL_A_SHIFT);
#else
	return 255;
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

INLINE u32 Texture::GetUsedMemory() const
{
	u32 size = 4;

	if (pixelFormat == kTexture2DPixelFormat_RGB565)
		size = 2;

	return size * iWidth * iHeight;
}

INLINE f32 Texture::GetWidth() const
{
	return fWidth;
}

INLINE f32 Texture::GetHeight() const
{
	return fHeight;
}

INLINE u32 Texture::GetWidthInPixel() const
{
	return this->iWidth;
}

INLINE u32 Texture::GetHeightInPixel() const
{
	return this->iHeight;
}

INLINE int Texture::LoadTexture()
{
	if (pImage && !iTextureId)
	{
		GLint saveName;

		glGenTextures(1, &iTextureId);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
		glBindTexture(GL_TEXTURE_2D, iTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (bCompressed)
		{
			GLuint bpp = 2;
			GLsizei size = this->iWidth * this->iHeight * bpp / 8;

			if (size < 32)
			{
				size = 32;
			}


			glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, this->iWidth, this->iHeight, 0, size, this->pImage);
		}
		else
		{
			switch (pixelFormat)
			{
				case kTexture2DPixelFormat_RGBA8888:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->iWidth, this->iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->pImage);
				break;

				case kTexture2DPixelFormat_RGB565:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->iWidth, this->iHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, this->pImage);
				break;

				case kTexture2DPixelFormat_A8:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, this->iWidth, this->iHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, this->pImage);
				break;

				default:
				break;
			}
		}
		glBindTexture(GL_TEXTURE_2D, saveName);
	}

	return this->iTextureId;
}

INLINE int Texture::GetTexture()
{
	return this->iTextureId;
}

INLINE void Texture::UnloadTexture()
{
	if (iTextureId)
		glDeleteTextures(1, &iTextureId);

	iTextureId = 0;
}

// FIXME: 2009-02-15 | Use Width x Height from image. | Danny Angelo Carminati Grein
void Texture::LoadPVRTC(const char *file)
{
	this->iHeight = 1024;
	this->iWidth = 1024;
	pFileSystem->Open(file, &stFile, this->pPool);
	this->pImage = stFile.GetData();
	this->bCompressed = TRUE;

//#if defined(ENABLE_PRELOAD_TEXTURE)
	this->LoadTexture();
//#endif // ENABLE_PRELOAD_TEXTURE

	this->iWidth = 1024;
	this->iHeight = 1024;
	this->pixelFormat = kTexture2DPixelFormat_RGBA2;
	this->fWidth = 1024 / pScreen->GetWidth();
	this->fHeight = 1024 / pScreen->GetHeight();
}

void Texture::LoadPNG(const char *file)
{
	NSUInteger width, height, i;
	CGContextRef context = nil;
	void *data = nil;;
	CGColorSpaceRef colorSpace;
	void *tempData;
	unsigned int *inPixel32;
	unsigned short *outPixel16;
	BOOL hasAlpha;
	CGImageAlphaInfo info;
	CGAffineTransform transform;
	CGSize imageSize;
	eTextureFormat pixelFormat;
	CGImageRef image;
	UIImageOrientation orientation;
	BOOL sizeToFit = NO;


	NSString *fname = [NSString stringWithCString:file encoding: [NSString defaultCStringEncoding]];
	NSString *path = [@"/data/" stringByAppendingString:fname];
	UIImage *uiImage = [UIImage imageNamed:path];

	if (NULL == uiImage)
	{
		Log("WARNING: Image file %s not found!!!!!!", file);
		iTextureId = 0;
		return;
	}

	image = [uiImage CGImage];
	orientation = [uiImage imageOrientation];

	if (image == NULL) return;

	info = CGImageGetAlphaInfo(image);
	hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);

	if (CGImageGetColorSpace(image))
	{
		if (hasAlpha)
			pixelFormat = kTexture2DPixelFormat_RGBA8888;
		else
			pixelFormat = kTexture2DPixelFormat_RGB565;
	}
	else  //NOTE: No colorspace means a mask image
		pixelFormat = kTexture2DPixelFormat_A8;

	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	transform = CGAffineTransformIdentity;
	width = imageSize.width;

	if ((width != 1) && (width & (width - 1)))
	{
		i = 1;
		while ((sizeToFit ? 2 * i : i) < width) i *= 2;
		width = i;
	}

	height = imageSize.height;

	if ((height != 1) && (height & (height - 1)))
	{
		i = 1;
		while ((sizeToFit ? 2 * i : i) < height) i *= 2;
		height = i;
	}

	while ((width > MAX_TEXTURE_SIZE) || (height > MAX_TEXTURE_SIZE))
	{
		width /= 2;
		height /= 2;
		transform = CGAffineTransformScale(transform, static_cast<f32>(0.5), static_cast<f32>(0.5));
		imageSize.width *= static_cast<f32>(0.5);
		imageSize.height *= static_cast<f32>(0.5);
	}

	switch (pixelFormat)
	{
		case kTexture2DPixelFormat_RGBA8888:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = pMemoryManager->Alloc(height * width * 4, this->pPool);
			context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
		break;

		case kTexture2DPixelFormat_RGB565:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = pMemoryManager->Alloc(height * width * 4, this->pPool);
			context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
		break;

		case kTexture2DPixelFormat_A8:
			data = pMemoryManager->Alloc(height * width, this->pPool);
			context = CGBitmapContextCreate(data, width, height, 8, width, NULL, kCGImageAlphaOnly);
		break;

		default:
		break;
	}

	CGContextClearRect(context, CGRectMake(0, 0, width, height));
	CGContextTranslateCTM(context, 0, height - imageSize.height);

	if (!CGAffineTransformIsIdentity(transform))
		CGContextConcatCTM(context, transform);

	CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
	if (pixelFormat == kTexture2DPixelFormat_RGB565)
	{
		tempData = pMemoryManager->Alloc(height * width * 2, this->pPool);
		inPixel32 = (unsigned int *)data;
		outPixel16 = (unsigned short *)tempData;

		for (i = 0; i < width * height; ++i, ++inPixel32)
			*outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);

		pMemoryManager->Free(data, pPool);
		data = tempData;
	}

	this->pImage = data;
	this->bCompressed = FALSE;
	this->iWidth = width;
	this->iHeight = height;
	//this->iWidth = imageSize.width;
	//this->iHeight = imageSize.height;
	this->pixelFormat = pixelFormat;
	this->fWidth = imageSize.width / pScreen->GetWidth();
	this->fHeight = imageSize.height / pScreen->GetHeight();

	//this->LoadTexture();

	CGContextRelease(context);

	//Log(">>>>>>>>>>>> MEM: %s == %d == Texture ID: %d", file, (width * height * 4), iTextureId);

	//pImage = data;
//#if SEED_ENABLE_KEEP_IMAGE_DATA == 0 // Release texture memory?
//	pMemoryManager->Free(const_cast<void *>(pImage), pPool);
//	pImage = NULL;
//#endif // SEED_ENABLE_KEEP_IMAGE_DATA
}

}} // namespace

#endif // _IPHONE_
