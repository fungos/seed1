/*! \file Text.h
	\author	Danny Angelo Carminati Grein
	\brief A text class
*/

#ifndef __TEXT_H__
#define __TEXT_H__

#include "Font.h"
#include "MemoryManager.h"
#include "TString.h"


#include <stdarg.h>

namespace Seed {


class Text
{
	public:
		Text();
		virtual ~Text();

		void Reset();

		void SetText(u32 dictStr);
		void SetText(WideString str);
		void SetText(const String &str);
/*
		void Print(WideString str, ...);
		void Print(WideString str, va_list *ap);
		f32 GetWidth(WideString str, ...);
*/
		//void Draw();
		void Draw(u32 index = 0, u32 size = 0);

		//f32 GetWidth(WideString str);
		f32 GetWidth(u32 index, u32 size);
		u32 GetLengthNonBreakMaxWidth(u32 *index, f32 maxWidth, f32 *lineWidth);

		//virtual BOOL IsChanged();
		virtual f32 GetFontTracking() const;
		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual f32 GetX() const;
		virtual f32 GetY() const;

		//virtual void SetChanged(BOOL bChanged);
		virtual void SetPosition(f32 x, f32 y);
		virtual void SetFont(Font *f);
		virtual void SetBlending(IRenderable::eBlendMode op);
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(PIXEL px);
		virtual void SetScale(f32 scaleX, f32 scaleY);

		virtual WideString GetText() const;

	private:
		SEED_DISABLE_COPY(Text);

	private:
		Font		*pFont;
		WideString	pStrData;

		f32 fPosX;
		f32 fPosY;
		f32 fWidth;
		f32 fHeight;
		f32 fScaleX;
		f32 fScaleY;

		BOOL bStatic;
		//BOOL bChanged;

		PIXEL iColor;
		IRenderable::eBlendMode iOperation;
};


} // namespace


#endif // __TEXT_H__
