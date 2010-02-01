/*! \file TextArea.h
	\author Danny Angelo Carminati Grein
	\brief GUI TextArea Widget
*/

#ifndef __GUI_TEXTAREA_H__
#define __GUI_TEXTAREA_H__

#include "interface/IWidget.h"
#include "Text.h"
#include "GuiManager.h"


namespace Seed {


class TextArea : public IWidget
{
	public:
		TextArea();
		virtual ~TextArea();

		virtual void Reset();
		virtual void Update(f32 dt);

		virtual void SetText(u32 dictId);
		virtual void SetText(WideString str);
		virtual void SetText(const String &str);
		//virtual void Print(WideString str, ...);
		virtual void SetFont(const Font *font);

		virtual void SetPriority(u32 p);
		virtual void SetAlignment(eHorizontalAlignment align);
		virtual void SetAlignment(eVerticalAlignment align);
		virtual void SetAutoAdjust(BOOL b);

		virtual u32 GetLineCount() const;

		// IRenderable
		virtual void Render(f32 delta);
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(PIXEL px);
		virtual PIXEL GetColor() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		SEED_DISABLE_COPY(TextArea);

		virtual void Rebuild();
		virtual u32 CalculateLineCount();

		virtual u32 RebuildPosX();
		virtual void RebuildPosY(u32 usedLines);

	protected:
		struct sLineInfo
		{
			f32 fPosX;
			f32 fPosY;
			u32 iIndex;
			u32 iSize;
		};

	protected:
		u32			iLines;

		f32			fDiffX;
		f32			fScaleX;
		f32			fScaleY;

		PIXEL		iColor;

		eHorizontalAlignment	eHAlign;
		eVerticalAlignment		eVAlign;

		const Font	*pFont;
		sLineInfo	*pLines;

		Text		cText;

		BOOL		bAutoAdjust;
};


} // namespace


#endif // __GUI_TEXTAREA_H__

