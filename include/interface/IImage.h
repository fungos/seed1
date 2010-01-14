/*! \file IImage.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Image class interface
*/

#ifndef __IIMAGE_H__
#define __IIMAGE_H__

#include "Defines.h"
#include "IResource.h"
#include "MemoryManager.h"
#include "Enum.h"


namespace Seed {


class IImage : public IResource
{
	public:
		IImage();
		virtual ~IImage();

		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;
		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		// IResource
		virtual BOOL Unload();
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool);

		/// Create a texture using a pre-allocated buffer.
		/**
		Create a texture using a pre-allocated buffer provided by the user, the
		Image class won't deallocate it uppon Unloading, so keep sure to clean your
		buffers when not needed anymore. And the same way, keep sure that the buffer
		will exist while Image exists.

		\param width Width of the image
		\param height Height of the image
		\param buffer Buffer to image pixels
		\param pool Pointer to IMemoryPool to use when allocating this object
		*/
		//virtual BOOL Load(u16 width, u16 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool) = 0;

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [] (void *);

	protected:
		eTextureFilter		nMinFilter;
		eTextureFilter		nMagFilter;

	private:
		SEED_DISABLE_COPY(IImage);
};


} // namespace


#endif // __IIMAGE_H__
