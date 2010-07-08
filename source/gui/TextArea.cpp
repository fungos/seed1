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

/*! \file TextArea.cpp
	\author Danny Angelo Carminati Grein
	\brief GUI TextArea Widget
*/

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "Rect.h"
#include "gui/TextArea.h"
#include "ViewManager.h"
#include "Screen.h"
#include "Number.h"
#include "RendererDevice.h"

#define TAG		"[GUI::TextArea] "

namespace Seed {

TextArea::TextArea()
	: IWidget()
	, iLines(0)
	, fDiffX(0.0f)
	, fTextScaleX(1.0f)
	, fTextScaleY(1.0f)
	, eHAlign(HorizontalAlignLeft)
	, eVAlign(VerticalAlignTop)
	, pFont(NULL)
	, pLines(NULL)
	, cText()
	, bAutoAdjust(FALSE)
{
}

TextArea::~TextArea()
{
	if (Private::bInitialized)
		pGuiManager->Remove(this);
}

void TextArea::Reset()
{
	if (pLines)
		pMemoryManager->Free(pLines);

	iId				= 0;
	bDisabled		= TRUE;
	iColor.pixel	= 0;
	iLines			= 0;
	eHAlign 		= HorizontalAlignLeft;
	eVAlign 		= VerticalAlignTop;
	eBlendOperation	= BlendNone;
	pLines			= NULL;

	IWidget::Reset();
}

void TextArea::ReleaseText()
{
	if (pLines)
		pMemoryManager->Free(pLines);

	pLines			= NULL;
	iId				= 0;
	bDisabled		= TRUE;
	iColor.pixel	= 0;
	iLines			= 0;
	eHAlign 		= HorizontalAlignLeft;
	eVAlign 		= VerticalAlignTop;
	eBlendOperation	= Seed::BlendNone;

	cText.Reset();
}

void TextArea::Update(f32 dt)
{
	UNUSED(dt);

	if (bChanged)
	{
		this->Rebuild();
	}

	bChanged = FALSE;
	bTransformationChanged = FALSE;
}

void TextArea::Render()
{
	cText.SetBlending(eBlendOperation);
	cText.SetColor(iColor.pixel);
	cText.SetScale(fTextScaleX, fTextScaleY);

	for (u32 i = 0; i < iLines; i++)
	{
		if (!pLines[i].iIndex && !pLines[i].iSize)
			break;

		cText.SetPosition(pLines[i].fPosX + this->GetX(), pLines[i].fPosY + this->GetY());
		cText.Draw(pLines[i].iIndex, pLines[i].iSize);
	}

	if (pConfiguration->bDebugText)
		pRendererDevice->DrawRect(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), PIXEL_COLOR(255, 0, 0, 255));
}

u32 TextArea::GetLineCount() const
{
	return iLines;
}

u32 TextArea::CalculateLineCount()
{
	u32 lines = 0;
	if (bAutoAdjust)
	{
		u32 index = 0;
		f32 lineWidth = 0;
		u32 size = 0;
		while (1)
		{
			size = cText.GetLengthNonBreakMaxWidth(&index, this->GetWidth(), &lineWidth);

			if (!size)
				break;

			lines++;
			index += size;
		}
		IWidget::SetHeight(lines * (cText.GetHeight() + cText.GetFontTracking()));
	}
	else
	{
		lines = static_cast<u32>(this->GetHeight() / (cText.GetHeight() + cText.GetFontTracking()));
	}

	return lines;
}

void TextArea::Rebuild()
{
	u32 lines = this->CalculateLineCount();
	if (!lines)
		return;

	u32 sizeBuff = lines * sizeof(struct sLineInfo);
	if (lines != iLines)
	{
		iLines = lines;

		pMemoryManager->Free(pLines);
		pLines = static_cast<struct sLineInfo *>(pMemoryManager->Alloc(sizeBuff, pDefaultPool));
	}
	MEMSET(pLines, '\0', sizeBuff);

	u32 usedLines = RebuildPosX();

	RebuildPosY(usedLines);
}

u32 TextArea::RebuildPosX()
{
	u32 index = 0;
	u32 size = 0;
	f32 lineWidth = 0.0f;
	u32 usedLines = 0;

	cText.SetScale(fTextScaleX, fTextScaleY);
	for (u32 i = 0; i < iLines; i++)
	{
		size = cText.GetLengthNonBreakMaxWidth(&index, this->GetWidth(), &lineWidth);

		if (!size)
			break;

		pLines[i].iSize = size;
		pLines[i].iIndex = index;

		f32 diffX = this->GetWidth() - lineWidth;
		switch (eHAlign)
		{
			case HorizontalAlignLeft:
				pLines[i].fPosX = 0.0f;
			break;

			case HorizontalAlignRight:
				pLines[i].fPosX = diffX;
			break;

			case HorizontalAlignCenter:
				pLines[i].fPosX = (diffX / 2.0f);
			break;

			case HorizontalAlignNone:
			default:
			break;
		}

		index += size;
		usedLines++;
	}

	return usedLines;
}

void TextArea::RebuildPosY(u32 usedLines)
{
	f32 y = 0.0f;
	f32 textHeight = (cText.GetHeight() + cText.GetFontTracking()) * usedLines;
	switch (eVAlign)
	{
		case VerticalAlignCenter:
			y += ((this->GetHeight() - textHeight) / 2.0f);
		break;

		case VerticalAlignBottom:
			y += this->GetHeight() - textHeight;
		break;

		case VerticalAlignTop:
		default:
		break;
	}

	for (u32 i = 0; i < usedLines; i++)
		pLines[i].fPosY = y + (cText.GetHeight() + cText.GetFontTracking()) * i;
}

INLINE void TextArea::SetText(const WideString str)
{
	cText.SetScale(fTextScaleX, fTextScaleY);
	cText.SetText(str);

	if (!this->GetHeight())
		IWidget::SetHeight(cText.GetHeight() + cText.GetFontTracking());

	if (!this->GetWidth())
		IWidget::SetWidth(cText.GetWidth());

	this->Rebuild();
}

INLINE void TextArea::SetText(const String &str)
{
	cText.SetScale(fTextScaleX, fTextScaleY);
	cText.SetText(str);

	if (!this->GetHeight())
		IWidget::SetHeight(cText.GetHeight() + cText.GetFontTracking());

	if (!this->GetWidth())
		IWidget::SetWidth(cText.GetWidth());

	this->Rebuild();
}

void TextArea::SetScaleX(f32 scaleX)
{
	this->SetScale(scaleX, this->GetScaleY());
}

void TextArea::SetScaleY(f32 scaleY)
{
	this->SetScale(this->GetScaleX(), scaleY);
}

void TextArea::SetScale(f32 scale)
{
	this->SetScale(scale, scale);
}

void TextArea::SetScale(f32 scaleX, f32 scaleY)
{
	fTextScaleX = scaleX;
	fTextScaleY = scaleY;

	cText.SetScale(scaleX, scaleY);

	if (bAutoAdjust)
	{
		if (this->GetWidth() != cText.GetWidth())
			IWidget::SetWidth(cText.GetWidth());

		if (this->GetHeight() != cText.GetHeight())
			IWidget::SetHeight(cText.GetHeight());
	}

	bChanged = TRUE;
}

void TextArea::SetScale(const Point<f32> &scale)
{
	this->SetScale(scale.x, scale.y);
}

void TextArea::AddScaleX(f32 scaleX)
{
	this->SetScale(this->GetScaleX() + scaleX, this->GetScaleY());
}

void TextArea::AddScaleY(f32 scaleY)
{
	this->SetScale(this->GetScaleX(), this->GetScaleY() + scaleY);
}

void TextArea::AddScale(f32 scale)
{
	this->SetScale(this->GetScaleX() + scale, this->GetScaleY() + scale);
}

void TextArea::AddScale(f32 scaleX, f32 scaleY)
{
	this->SetScale(this->GetScaleX() + scaleX, this->GetScaleY() + scaleY);
}

void TextArea::AddScale(const Point<f32> &scale)
{
	this->SetScale(this->GetScaleX() + scale.x, this->GetScaleY() + scale.y);
}

INLINE void TextArea::SetWidth(f32 w)
{
	IWidget::SetWidth(w);

	bAutoAdjust = FALSE;
	bChanged = TRUE;
}

INLINE void TextArea::SetHeight(f32 h)
{
	IWidget::SetHeight(h);

	bAutoAdjust = FALSE;
	bChanged = TRUE;
}

INLINE f32 TextArea::GetScaleX() const
{
	return cText.GetScaleX();
}

INLINE f32 TextArea::GetScaleY() const
{
	return cText.GetScaleY();
}

INLINE void TextArea::SetAutoAdjust(BOOL b)
{
	bAutoAdjust = b;
}

INLINE void TextArea::SetFont(const Font *font)
{
	pFont = font;
	cText.SetFont(const_cast<Font*>(pFont));
	bChanged = TRUE;
}

INLINE void TextArea::SetPriority(u32 p)
{
	ITransformable2D::iPriority = p;
	IWidget::iPriority = p;
}

INLINE void TextArea::SetAlignment(eHorizontalAlignment align)
{
	eHAlign = align;
	bChanged = TRUE;
}

INLINE void TextArea::SetAlignment(eVerticalAlignment align)
{
	eVAlign = align;
	bChanged = TRUE;
}

INLINE const char *TextArea::GetObjectName() const
{
	return "TextArea";
}

INLINE int TextArea::GetObjectType() const
{
	return Seed::ObjectGuiTextArea;
}

} // namespace

