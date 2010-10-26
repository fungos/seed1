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
#include "RendererDevice.h"

#undef WideChar
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

#include <math.h>

#define TAG	"[Texture] "
#define MAX_TEXTURE_SIZE	1024

namespace Seed { namespace iPhone {

IResource *TextureResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);

	Texture *image = New(Texture());
	image->Load(filename, res, pool);

	return image;
}

Texture::Texture()
	: pData(NULL)
	, pTextureId(NULL)
	, iBytesPerPixel(4)
	, iPitch(0)
	, iAtlasWidth(0)
	, iAtlasHeight(0)
	, iReserved(0)
	//, bCompressed(FALSE)
	, pixelFormat(kTexture2DPixelFormat_RGBA8888)
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
	
	pData = NULL;
	pPool = NULL;
	
	iBytesPerPixel = 4;
	iPitch = 0;
	iAtlasWidth = 0;
	iAtlasHeight = 0;

	//bCompressed = FALSE;
	pixelFormat = kTexture2DPixelFormat_RGBA8888;
}

INLINE BOOL Texture::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	if (ITexture::Load(filename, res, pool))
	{
		stFile.Close();
		//#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
		this->LoadPNG(filename);
		//#else
		//this->LoadPVRTC(filename);
		//#endif // ENABLE_NATIVE_PVRTC_FORMAT
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
		pFilename = "[dynamic texture]";
		
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
		pixelFormat = kTexture2DPixelFormat_RGBA8888;

		pRendererDevice->TextureRequest(this, &pTextureId);

		bLoaded = TRUE;
	}
	
	return bLoaded;
}

INLINE void Texture::Update(PIXEL *data)
{
	pData = data;
	pRendererDevice->TextureDataUpdate(this);
}

INLINE BOOL Texture::Unload()
{
	this->UnloadTexture();

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
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (pData || pixelFormat != kTexture2DPixelFormat_RGB565 || pixelFormat != kTexture2DPixelFormat_A8)
	{
		const PIXEL *data1 = static_cast<const PIXEL *>(pData);
		PIXEL *data = const_cast<PIXEL *>(data1);
		data[(y * iWidth) + x] = px; // ja deve ser arrumado em relacao ao atlas
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
	if (!pData)
		return 0;

	if (pixelFormat == kTexture2DPixelFormat_RGB565 || pixelFormat == kTexture2DPixelFormat_A8)
	{
		Log(TAG "GetPixel unsuported for format 565 (yet).");
		return 0;
	}

	const PIXEL *data = static_cast<const PIXEL *>(pData);
	PIXEL px = data[(y * iWidth) + x]; // ja deve ser arrumado em relacao ao atlas

	return px;
#else
	return 0;
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

INLINE u8 Texture::GetPixelAlpha(u32 x, u32 y) const
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (!pData)
		return 255;

	if (pixelFormat == kTexture2DPixelFormat_RGB565 || pixelFormat == kTexture2DPixelFormat_A8)
	{
		Log(TAG "GetPixel unsuported for format 565 (yet).");
		return 255;
	}

	if (x >= iWidth)
	{
		x = static_cast<u32>(iAtlasWidth) - 1;
		return this->GetPixelAlpha(x, y);
	}
	else if (y >= iHeight)
	{
		y = static_cast<u32>(iAtlasHeight) - 1;
		return this->GetPixelAlpha(x, y);
	}

	PIXEL px = this->GetPixel(x, y);

	return ((PIXEL_A_MASK & px) >> PIXEL_A_SHIFT);
#else
	return 255;
#endif // ENABLE_NATIVE_PVRTC_FORMAT
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

	if (pData)
		pMemoryManager->Free(pData, pPool);
	pData = NULL;
	
	if (iReserved)
		pMemoryManager->Unreserve(iReserved, pDefaultPool);
	iReserved = 0;
	pTextureId = NULL;
}

INLINE void Texture::Close()
{
	Log("WARNING: Releasing texture buffer for %s, but keeping it size (%d) reserved in memory manager - this can cause memory fragmentation in platforms that VRAM and RAM are separated!", pFilename, iReserved);
	pMemoryManager->Reserve(iReserved, pDefaultPool);
	stFile.Close();

//#if !defined(DEBUG)
	if (pData)
		pMemoryManager->Free(pData, pPool);
	pData = NULL;
//#endif
}

// FIXME: 2009-02-15 | Use Width x Height from image. | Danny Angelo Carminati Grein
/*
void Texture::LoadPVRTC(const char *file)
{
	iHeight = 1024;
	iWidth = 1024;
	pFileSystem->Open(file, &stFile, pPool);
	pImage = stFile.GetData();
	bCompressed = TRUE;

//#if defined(ENABLE_PRELOAD_TEXTURE)
	this->LoadTexture();
//#endif // ENABLE_PRELOAD_TEXTURE

	iWidth = 1024;
	iHeight = 1024;
	pixelFormat = kTexture2DPixelFormat_RGBA2;
	fWidth = 1024 / pScreen->GetWidth();
	fHeight = 1024 / pScreen->GetHeight();
}
*/

void Texture::LoadPNG(const char *file)
{
	NSUInteger width, height, i;
	CGContextRef context = nil;
	void *data = nil;;
	CGColorSpaceRef colorSpace;
	CGColorSpaceRef srcColorSpace;
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
		return;
	}

	image = [uiImage CGImage];
	orientation = [uiImage imageOrientation];

	if (image == NULL) return;

	info = CGImageGetAlphaInfo(image);
	hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);

	srcColorSpace = CGImageGetColorSpace(image);
	
	// FIXME: Enable support to grayscale pngs.
	ASSERT_MSG(srcColorSpace != CGColorSpaceCreateDeviceGray(), "WARNING: Grayscale PNG are not supported.\n");
	
	if (srcColorSpace)
	{
		if (hasAlpha)
		{
			pixelFormat = kTexture2DPixelFormat_RGBA8888;
		}
		else
		{
			pixelFormat = kTexture2DPixelFormat_RGB565;
		}
	}
	else  //NOTE: No colorspace means a mask image
	{
		pixelFormat = kTexture2DPixelFormat_A8;
	}
	
	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	transform = CGAffineTransformIdentity;
	
	iWidth = imageSize.width;
	iHeight = imageSize.height;
	
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

	u32 bpc = CGImageGetBitsPerComponent(image);
	u32 bpp = CGImageGetBitsPerPixel(image);
	iBytesPerPixel = bpp / 8;
	data = pMemoryManager->Alloc(height * width * iBytesPerPixel, pPool, file, "iOS Texture");
	ASSERT_MSG(data != NULL, "ERROR: not enought memory - trying to allocate texture buffer.");
	
	switch (pixelFormat)
	{
		case kTexture2DPixelFormat_RGBA8888:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			context = CGBitmapContextCreate(data, width, height, bpc, iBytesPerPixel * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
		break;

		case kTexture2DPixelFormat_RGB565:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			context = CGBitmapContextCreate(data, width, height, bpc, iBytesPerPixel * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
		break;

		case kTexture2DPixelFormat_A8:
			context = CGBitmapContextCreate(data, width, height, bpc, iBytesPerPixel * width, NULL, kCGImageAlphaOnly);
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
		tempData = pMemoryManager->Alloc(height * width * 2, pPool, file, "iOS Texture");
		inPixel32 = (unsigned int *)data;
		outPixel16 = (unsigned short *)tempData;

		for (i = 0; i < width * height; ++i, ++inPixel32)
			*outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);

		pMemoryManager->Free(data, pPool);
		data = tempData;
	}

	pData = data;
	//bCompressed = FALSE;
	pixelFormat = pixelFormat;
	
	// FIXME: Must divide by res_width , res_height - not by screen width/height
	fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
	fHeight = (f32)iHeight / (f32)pScreen->GetHeight();

	iAtlasWidth = width;
	iAtlasHeight = height;
	iWidth = imageSize.width;
	iHeight = imageSize.height;
	iPitch = CGImageGetBytesPerRow(image);
	iReserved = height * width * iBytesPerPixel;

	pRendererDevice->TextureRequest(this, &pTextureId);

	CGContextRelease(context);

	//Log(">>>>>>>>>>>> MEM: %s == %d == Texture ID: %d", file, (width * height * 4), iTextureId);
}

}} // namespace

#endif // _IPHONE_
