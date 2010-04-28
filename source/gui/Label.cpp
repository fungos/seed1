/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

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

	if (Private::bInitialized)
		pGuiManager->Remove(this);
}

void Label::Reset()
{
	this->iId				= 0;
	this->iColor			= 0;
	this->eBlendOperation	= Seed::BlendNone;

	IWidget::Reset();

	this->bVisible			= TRUE;
	this->bChanged			= TRUE;
	this->bTransformationChanged = TRUE;
	this->bDisabled			= TRUE;
	this->bAutoAdjust		= TRUE;
	this->iAlign			= HorizontalAlignLeft;

	this->cText.Reset();
}

void Label::ReleaseText()
{
	this->iId				= 0;
	this->iColor			= 0;
	this->eBlendOperation	= IRenderable::NONE;

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
		//IWidget::Update(dt);
		UNUSED(dt);

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

INLINE void Label::SetText(const WideString str)
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
