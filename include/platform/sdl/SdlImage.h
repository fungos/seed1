/*! \file SdlImage.h
	\author	Danny Angelo Carminati Grein
	\brief Image SDL Implementation
*/


#ifndef __SDL_IMAGE_H__
#define __SDL_IMAGE_H__


#ifdef _SDL_

#include "Defines.h"
#include "File.h"
#include "interface/IImage.h"
#include "SeedInit.h"

namespace Seed { namespace OGL { class Sprite; }}

namespace Seed { namespace SDL {


IResource *ImageResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);


class Image : public IImage
{
	friend IResource *ImageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class OGL::Sprite;

	public:
		Image();
		virtual ~Image();

		// IImage
		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		virtual void Update();
		virtual void Reset();
		virtual BOOL Load(u32 width, u32 height, PIXEL *buffer, IMemoryPool *pool = pDefaultPool); // O que acontece no Reload?

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		int LoadTexture();
		int GetTexture();
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Image);

	private:
		SDL_Surface *pSurface;
		void *pData;

#ifdef DEBUG
		File stFile;
#endif
		GLuint iTextureId;

		u32 iBytesPerPixel;
		u32 iPitch;
};


}} // namespace


#else // _SDL_

	#error "Include 'Image.h' instead 'platform/sdl/SdlImage.h' directly."

#endif // _SDL_
#endif // __SDL_IMAGE__
