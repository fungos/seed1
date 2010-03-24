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
		virtual BOOL Load(u32 width, u32 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool);
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
		QImage image;

		u32 iTextureId;

		s32 iHalfWidth;
		s32 iHalfHeight;
};

}} // namespace

#else // _QT_
	#error "Include 'Image.h' instead 'platform/qt/QtImage.h' directly."
#endif // _QT_
#endif // __QT_IMAGE__
