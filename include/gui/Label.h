/*! \file Label.h
	\author Danny Angelo Carminati Grein
	\brief GUI Label Widget
*/

#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

#include "Text.h"
#include "GuiManager.h"
#include "../interface/IWidget.h"


namespace Seed {


class Label : public IWidget
{
	public:
		Label();
		virtual ~Label();

		virtual void Reset();
		virtual void Update(f32 dt);

		virtual void SetAutoAdjust(BOOL b);
		virtual BOOL IsAutoAdjust() const;
		virtual void SetAlignment(eHorizontalAlignment align);
		virtual void SetText(u32 dictId);
		virtual void SetText(WideString str);
		virtual void SetText(const String &str);
		//virtual void Print(WideString str, ...);
		virtual void SetFont(const Font *font);

		f32 GetTextWidth();

		// IRenderable
		virtual void Render(f32 delta);
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(PIXEL px);

		// ITransformable2D
		virtual void SetWidth(f32 w);

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		eHorizontalAlignment	iAlign;
		PIXEL		iColor;
		BOOL		bAutoAdjust;
		Text		cText;
		const Font	*pFont;

	private:
		SEED_DISABLE_COPY(Label);
};


} // namespace


#endif // __LABEL_H__

