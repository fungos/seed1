/*! \file Font.h
	\author	Danny Angelo Carminati Grein
	\brief A font class
*/


#ifndef __FONT_H__
#define __FONT_H__


#include "Defines.h"
#include "interface/IResource.h"
#include "interface/IMemoryManager.h"
#include "interface/IRenderable.h"
#include "Sprite.h"
#include "Enum.h"


namespace Seed {


IResource *FontResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);


class Font : public IResource
{
	friend IResource *FontResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class Text;

	public:
		Font();
		virtual ~Font();

		const void *GetLetter(u32 letter);
		void SetLetter(u32 letter);

		f32 GetWidth() const;
		f32 GetHeight() const;
		void SetSpacing(f32 spacing);
		f32 GetSpacing() const;
		void SetSpaceWidth(f32 width);
		f32 GetSpaceWidth() const;
		void SetTracking(f32 tracking);
		f32 GetTracking();
		void SetPosition(f32 x, f32 y);

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [](void *);

		void SetBlending(IRenderable::eBlendMode op);
		void SetColor(PIXEL px);
		void Draw();
		void Update(f32 dt);
		void SetScale(f32 x, f32 y);

	private:
		SEED_DISABLE_COPY(Font);
		u32 SelectLetter(u32 letter);

	private:
		File			stFile;
		Sprite			mFont;
		Sprite			mFontExt;
		Sprite			*pFont;

		f32				fSpacing;
		f32				fTracking;
		f32				fSpaceWidth;
		f32				fWidth;
		f32				fHeight;

		u32				iCurrent;
		u32				iTotalGlyphs;
		const u16		*pGlyphs;

		BOOL			bSpace;
};


} // namespace


#endif // __FONT_H__
