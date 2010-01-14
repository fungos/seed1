/*! \file WiiImage.h
	\author	Danny Angelo Carminati Grein
	\brief Image implementation
*/


#ifndef __WII_IMAGE_H__
#define __WII_IMAGE_H__


#include "Defines.h"


#if defined(_WII_)


#include "interface/IImage.h"
#include "ResourceManager.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "File.h"
#include "SeedInit.h"


namespace Seed { namespace WII {


IResource *ImageResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);


class Image : public IImage
{
	friend IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool = pDefaultPool);
	friend class Sprite;


	public:
		Image();
		virtual ~Image();

		// IImage
		//virtual void Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool);

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
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();		
		virtual u32 GetUsedMemory() const;


	protected:
		File 	stFile;

	private:
		SEED_DISABLE_COPY(Image);

	private:
		WiiTexObj pImage;

		u8 	*pImageBuffer;

		f32 fWidth;
		f32 fHeight;

		s32 iHalfWidth;
		s32 iHalfHeight;

		u16 iWidth;
		u16 iHeight;

		void LoadPalette(const char *name);
};


}} // namespace


#else // _WII_

	#error "Include 'Image.h' instead 'platform/wii/WiiImage.h' directly."

#endif // _WII_
#endif // __WII_IMAGE_H__
