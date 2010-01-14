/*! \file QtImage.h
	\author	Danny Angelo Carminati Grein
	\brief Image QT Implementation
*/


#ifndef __QT_IMAGE_H__
#define __QT_IMAGE_H__


#ifdef _QT_

#include "Defines.h"
#include "File.h"
#include "interface/IImage.h"
#include "SeedInit.h"

#include <QImage>

namespace Seed { namespace QT {


IResource *ImageResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);


class Image : public IImage
{
	friend IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class Sprite;


	public:
		Image();
		virtual ~Image();

		// IImage
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool);
		//virtual void Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		virtual BOOL Reset();

		// IResource
		virtual u32 GetUsedMemory() const;
		int LoadTexture();

	protected:
		int GetTexture();
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Image);

	private:
		IMemoryPool *pPool;
		QImage *pImage;

		u32 iTextureId;

		f32 fWidth;
		f32 fHeight;

		s32 iHalfWidth;
		s32 iHalfHeight;

		u16 iWidth;
		u16 iHeight;
};


}} // namespace


#else // _QT_

	#error "Include 'Image.h' instead 'platform/qt/QtImage.h' directly."

#endif // _QT_
#endif // __QT_IMAGE__
