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
#include "ViewManager.h"
#include "Screen.h"
#include "RendererDevice.h"

#define TAG		"[GUI::Label] "

namespace Seed {

Label::Label()
	: iAlign(Seed::HorizontalAlignLeft)
	, bAutoAdjust(TRUE)
	, fTextScaleX(1.0f)
	, fTextScaleY(1.0f)
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
	iId = 0;
	eBlendOperation	= BlendNone;

	IWidget::Reset();

	bVisible = TRUE;
	bChanged = TRUE;
	bTransformationChanged = TRUE;
	bDisabled = TRUE;
	bAutoAdjust = TRUE;
	iAlign = HorizontalAlignLeft;

	cText.Reset();
}

void Label::ReleaseText()
{
	iId = 0;
	eBlendOperation = Seed::BlendNone;

	bVisible = TRUE;
	bChanged = TRUE;
	bTransformationChanged = TRUE;
	bDisabled = TRUE;
	bAutoAdjust = TRUE;
	iAlign = HorizontalAlignLeft;

	cText.Reset();
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

void Label::Render()
{
	cText.SetBlending(eBlendOperation);
	cText.SetColor(iColor.pixel);
	cText.SetScale(GetScaleX(), GetScaleY());
	cText.SetScale(fTextScaleX, fTextScaleY);
	cText.SetPosition(this->GetX(), this->GetY());
	cText.SetLocalPosition(this->GetLocalX(), this->GetLocalY());

	switch (iAlign)
	{
		case HorizontalAlignRight:
		{
			cText.SetPosition(this->GetX() + ((this->GetWidth() - cText.GetWidth())), this->GetY());
		}
		break;

		case HorizontalAlignCenter:
		{
			cText.SetPosition(this->GetX() + ((this->GetWidth() - cText.GetWidth()) / 2.0f), this->GetY());
		}
		break;

		case HorizontalAlignNone:
		case HorizontalAlignLeft:
		default:
		break;
	}

	cText.Draw();

	if (pConfiguration->bDebugText)
		pRendererDevice->DrawRect(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), PIXEL_COLOR(0, 255, 0, 255));
}

void Label::SetScaleX(f32 scaleX)
{
	this->SetScale(scaleX, this->GetScaleY());
}

void Label::SetScaleY(f32 scaleY)
{
	this->SetScale(this->GetScaleX(), scaleY);
}

void Label::SetScale(f32 scale)
{
	this->SetScale(scale, scale);
}

void Label::SetScale(f32 scaleX, f32 scaleY)
{
	fTextScaleX = scaleX;
	fTextScaleY = scaleY;

	cText.SetScale(scaleX, scaleY);

	if (bAutoAdjust)
	{
		if (this->GetWidth() != cText.GetWidth())
			this->SetWidth(cText.GetWidth());
	}

	if (this->GetHeight() != cText.GetHeight())
		this->SetHeight(cText.GetHeight());

	bChanged = TRUE;
}

void Label::SetScale(const Point<f32> &scale)
{
	this->SetScale(scale.x, scale.y);
}

void Label::AddScaleX(f32 scaleX)
{
	this->SetScale(this->GetScaleX() + scaleX, this->GetScaleY());
}

void Label::AddScaleY(f32 scaleY)
{
	this->SetScale(this->GetScaleX(), this->GetScaleY() + scaleY);
}

void Label::AddScale(f32 scale)
{
	this->SetScale(this->GetScaleX() + scale, this->GetScaleY() + scale);
}

void Label::AddScale(f32 scaleX, f32 scaleY)
{
	this->SetScale(this->GetScaleX() + scaleX, this->GetScaleY() + scaleY);
}

void Label::AddScale(const Point<f32> &scale)
{
	this->SetScale(this->GetScaleX() + scale.x, this->GetScaleY() + scale.y);
}

INLINE f32 Label::GetScaleX() const
{
	return cText.GetScaleX();
}

INLINE f32 Label::GetScaleY() const
{
	return cText.GetScaleY();
}

INLINE void Label::SetAutoAdjust(BOOL b)
{
	bAutoAdjust = b;
	bChanged = TRUE;
}

INLINE BOOL Label::IsAutoAdjust() const
{
	return bAutoAdjust;
}

INLINE void Label::SetWidth(f32 w)
{
	bAutoAdjust = FALSE;
	IWidget::SetWidth(w);
	bChanged = TRUE;
}

INLINE void Label::SetAlignment(eHorizontalAlignment align)
{
	iAlign = align;
	bAutoAdjust = FALSE;
	bChanged = TRUE;
}

INLINE void Label::SetText(const WideString str)
{
	cText.SetText(str);

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	bChanged = TRUE;
}

INLINE void Label::SetText(const String &str)
{
	cText.SetText(str);

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	bChanged = TRUE;
}

INLINE void Label::SetFont(const Font *font)
{
	pFont = font;
	cText.SetFont(const_cast<Font*>(pFont));

	this->SetHeight(cText.GetHeight());
	if (!this->GetWidth() || bAutoAdjust)
		IWidget::SetWidth(cText.GetWidth());

	bChanged = TRUE;
}

INLINE f32 Label::GetTextWidth()
{
	f32 width = 0.0f;
	u32 idx = 0;
	if (pFont && this->GetWidth())
		cText.GetLengthNonBreakMaxWidth(&idx, 1.0f, &width);

	return width;
}

INLINE const char *Label::GetObjectName() const
{
	return "Label";
}

INLINE int Label::GetObjectType() const
{
	return Seed::ObjectGuiLabel;
}

} // namespace
