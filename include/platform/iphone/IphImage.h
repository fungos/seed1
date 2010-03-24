/*! \file IphImage.h
	\author	Danny Angelo Carminati Grein
	\brief Image Iphone Implementation
*/

#ifndef __IPH_IMAGE_H__
#define __IPH_IMAGE_H__

#ifdef _IPHONE_

#include "Defines.h"
#include "SeedInit.h"
#include "File.h"
#include "interface/IImage.h"
#include <OpenGLES/ES1/gl.h>

namespace Seed { namespace iPhone {

IResource *ImageResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);

class Image : public IImage
{
	friend IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class Sprite;

	public:
		Image();
		virtual ~Image();

		// IImage
		virtual void Load(const char *filename, IMemoryPool *pool = pDefaultPool);
		virtual void Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool);
		virtual void Unload();

		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		virtual void Reset();

		// IResource
		virtual u32 GetUsedMemory() const;

	protected:
		int LoadTexture();
		int GetTexture();
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Image);

		void LoadPVRTC(const char *file);
		void LoadPNG(const char *file);

	private:
		enum eTextureFormat
		{
			kTexture2DPixelFormat_Automatic = 0,
			kTexture2DPixelFormat_RGBA8888,
			kTexture2DPixelFormat_RGB565,
			kTexture2DPixelFormat_A8,
			kTexture2DPixelFormat_RGBA2,
		};

	private:
		const void	*pImage;
		File		stFile;

		GLuint iTextureId;

		f32 fWidth;
		f32 fHeight;

		s32 iHalfWidth;
		s32 iHalfHeight;

		u16 iWidth;
		u16 iHeight;

		BOOL bCompressed;

		eTextureFormat pixelFormat;
};

}} // namespace

#else // _IPHONE_
	#error "Include 'Image.h' instead 'platform/iphone/IphImage.h' directly."
#endif // _IPHONE_
#endif // __IPH_IMAGE_H__
