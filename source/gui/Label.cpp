/*! \file Label.cpp
	\author Danny Angelo Carminati Grein
	\brief GUI Label Widget
*/


#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "Rect.h"
#include "gui/Label.h"
#include "Renderer2D.h"
#include "Screen.h"

#define TAG		"[GUI::Label] "

#if DEBUG_ENABLE_RECT_LABEL == 1
#define DEBUG_LABEL_RECT DEBUG_RECT(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), DEBUG_RECT_COLOR_LABEL);
#else
#define DEBUG_LABEL_RECT
#endif

namespace Seed {


Label::Label()
	: iAlign(Seed::HorizontalAlignLeft)
	, iColor(0)
	, bAutoAdjust(FALSE)
	, cText()
	, pFont(NULL)
{
	this->Reset();
}

Label::~Label()
{
	this->Reset();

	if(Private::bInitialized)
		pGuiManager->Remove(this);
}

void Label::Reset()
{
	this->iId				= 0;
	this->iColor			= 0;
	this->eBlendOperation	= IRenderable::NONE;

	IWidget::Reset();

	this->bVisible			= TRUE;
	this->bChanged			= TRUE;
	this->bTransformationChanged = TRUE;
	this->bDisabled			= TRUE;
	this->bAutoAdjust		= TRUE;
	this->iAlign			= HorizontalAlignLeft;

	this->cText.Reset();
}

void Label::Update(f32 dt)
{
	if (bChanged || bTransformationChanged)
	{
		IWidget::Update(dt);

		if (bAutoAdjust)
		{
			this->SetWidth(cText.GetWidth());
		}

		bChanged = FALSE;
		bTransformationChanged = FALSE;
	}
}

void Label::Render(f32 delta)
{
	UNUSED(delta);

	cText.SetBlending(eBlendOperation);
	cText.SetColor(iColor);
	cText.SetScale(GetScaleX(), GetScaleY());

	switch (iAlign)
	{
		case HorizontalAlignLeft:
		{
			cText.SetPosition(this->GetX(), this->GetY());
		}
		break;

		case HorizontalAlignRight:
		{
			cText.SetPosition(this->GetX() + ((this->GetWidth() - cText.GetWidth())), this->GetY());
		}
		break;

		case HorizontalAlignCenter:
		{
			this->cText.SetPosition(this->GetX() + ((this->GetWidth() - cText.GetWidth()) / 2.0f), this->GetY());
		}
		break;

		case HorizontalAlignNone:
		default:
		break;
	}

	cText.Draw();

	DEBUG_LABEL_RECT;
}

INLINE void Label::SetAutoAdjust(BOOL b)
{
	this->bAutoAdjust = b;
	this->bChanged = TRUE;
}

INLINE BOOL Label::IsAutoAdjust() const
{
	return this->bAutoAdjust;
}

INLINE void Label::SetWidth(f32 w)
{
	this->bAutoAdjust = FALSE;
	IWidget::SetWidth(w);
	this->bChanged = TRUE;
}

INLINE void Label::SetAlignment(eHorizontalAlignment align)
{
	this->iAlign = align;
	this->bChanged = TRUE;
}

INLINE void Label::SetText(u32 dictId)
{
	this->cText.SetText(dictId);

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || this->bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	this->bChanged = TRUE;
}

INLINE void Label::SetText(WideString str)
{
	this->cText.SetText(str);

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || this->bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	this->bChanged = TRUE;
}

INLINE void Label::SetText(const String &str)
{
	this->cText.SetText(str);

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || this->bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	this->bChanged = TRUE;
}

/*
INLINE void Label::Print(WideString str, ...)
{
	va_list ap;
	va_start(ap, str);
	this->cText.Print(str, &ap);
	va_end(ap);

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || this->bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	this->bChanged = TRUE;
}
*/

INLINE void Label::SetFont(const Font *font)
{
	this->pFont = font;
	this->cText.SetFont(const_cast<Font*>(pFont));

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || this->bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	this->bChanged = TRUE;
}

INLINE void Label::SetColor(u8 r, u8 g, u8 b, u8 a)
{
	this->iColor = PIXEL_COLOR(r, g, b, a);
	this->bChanged = TRUE;
}

INLINE void Label::SetColor(PIXEL px)
{
	this->iColor = px;
	this->bChanged = TRUE;
}

INLINE const char *Label::GetObjectName() const
{
	return "Label";
}

INLINE int Label::GetObjectType() const
{
	return Seed::ObjectGuiLabel;
}

INLINE f32 Label::GetTextWidth()
{
	f32 width = 0.0f;
	u32 idx = 0;
	if (pFont && this->GetWidth())
		this->cText.GetLengthNonBreakMaxWidth(&idx, 1.0f, &width);

	return width;
}


} // namespace
