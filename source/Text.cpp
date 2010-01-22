/*! \file Text.cpp
	\author	Danny Angelo Carminati Grein
	\brief Text implementation
*/

#include "Defines.h"
#include "Log.h"
#include "Text.h"
#include "Font.h"
#include "Dictionary.h"
#include "Screen.h"
#include "TString.h"

#include <stdarg.h>
#include <stdio.h>


#define TAG "[Text] "

#define TEXT_BUFFER_MAX	2048


namespace Seed {


// Flawfinder: ignore
//static wchar_t _seed_internal_buff[TEXT_BUFFER_MAX];


Text::Text()
	: pFont(NULL)
	, pStrData(NULL)
	, fPosX(0)
	, fPosY(0)
	, fWidth(0)
	, fHeight(0)
	, fScaleX(1.0f)
	, fScaleY(1.0f)
	, bStatic(FALSE)
	//, bChanged(FALSE)
	, iColor(0)
	, iOperation(IRenderable::NONE)
{
}

Text::~Text()
{
	Reset();
}

INLINE void Text::Reset()
{
	if (!this->bStatic && this->pStrData)
		pMemoryManager->Free(this->pStrData, pDefaultPool);

	if (this->pFont)
		this->pFont->Release();

	this->pFont		= NULL;
	this->pStrData	= NULL;

	this->fPosX = 0;
	this->fPosY = 0;
	this->bStatic	= FALSE;
	//this->bChanged  = FALSE;
}

INLINE void Text::SetText(u32 dictText)
{
	if (!this->bStatic && this->pStrData)
		pMemoryManager->Free(this->pStrData, pDefaultPool);

	this->pStrData = pDictionary->GetString(dictText); //staticStr; FIXME TODO
	this->bStatic = TRUE;
	//this->bChanged = TRUE;
	this->fWidth = this->GetWidth(0, String::Length(pStrData));
}

INLINE void Text::SetText(WideString str)
{
	if (!this->bStatic && this->pStrData)
		pMemoryManager->Free(this->pStrData, pDefaultPool);

	this->pStrData = str; //staticStr; FIXME TODO
	this->bStatic = TRUE;
	//this->bChanged = TRUE;
	this->fWidth = this->GetWidth(0, String::Length(pStrData));
}

INLINE void Text::SetText(const String &str)
{
	this->SetText(str.GetData());
}

/*
void Text::SetText(String &str)
{
	if (this->bStatic)
	{
		this->pStrData	= NULL;
		this->bStatic	= FALSE;
	}

	if (this->pStrData)
		pMemoryManager->Free(this->pStrData, pDefaultPool);

	MEMSET(_seed_internal_buff, '\0', TEXT_BUFFER_MAX);

	// Flawfinder: ignore
	WVSNPRINTF(_seed_internal_buff, TEXT_BUFFER_MAX, str, *ap);

	SIZE_T size = wcslen(_seed_internal_buff) + 1;
	this->pStrData = static_cast<WideString>(pMemoryManager->Alloc(size * sizeof(wchar_t), pDefaultPool));
	ASSERT_NULL(this->pStrData);

	MEMCOPY(this->pStrData, _seed_internal_buff, size * sizeof(wchar_t));

	this->fWidth = this->GetWidth(0, (u32)size);
	//this->bChanged	= TRUE;
}

void Text::Print(WideString str, ...)
{
	va_list ap;
	va_start(ap, str);
	this->Print(str, &ap);
	va_end(ap);
}

f32 Text::GetWidth(WideString str, ...)
{
	va_list ap;

	MEMSET(_seed_internal_buff, '\0', TEXT_BUFFER_MAX);

	va_start(ap, str);
	// Flawfinder: ignore
	WVSNPRINTF(_seed_internal_buff, TEXT_BUFFER_MAX, str, ap);
	va_end(ap);

	SIZE_T size = wcslen(_seed_internal_buff) + 1;

	return this->GetWidth(0, (u32)size);
}
*/
/*
INLINE f32 Text::GetWidth(WideString str)
{
	u32 size = String::Length(str);
	return this->GetWidth(0, size);
}
*/
INLINE f32 Text::GetWidth(u32 index, u32 size)
{
	f32 w = 0.0f;
	if (pStrData && pFont)
	{
		for (u32 i = 0; i < size; i++)
		{
			u32 letter = pStrData[index + i];

			if (letter == '\0' || letter == '\n')
				break;

			pFont->SetLetter(letter);
			w += pFont->GetWidth() + pFont->GetSpacing();
		}

		w -= pFont->GetSpacing();
	}

	return w;
}

u32 Text::GetLengthNonBreakMaxWidth(u32 *index, f32 maxWidth, f32 *lineWidth)
{
	// Pode ter somente texto ou somente fonte, por isso nao dar ASSERT
	if (!pStrData || !pFont)
		return 0;

	u32 size = String::Length(this->pStrData);
	if (*index >= size)
		return 0;

	size++; // \0
	f32 w = 0;
	u32 len = 0;
	u32 separator = 0;

	//Log("B index: %d  char: '%c'", *index, this->pStrData[*index]);
	if (this->pStrData[*index] == ' ' || this->pStrData[*index] == '\t' || this->pStrData[*index] == '\n' || this->pStrData[*index] == '\r')
		(*index)++;
	if (this->pStrData[*index] == '\r')
		(*index)++;

	//Log("A index: %d  char: '%c'", *index, this->pStrData[*index]);
	f32 nw = 0;
	for (u32 i = *index; i < size; i++)
	{
		u32 letter = this->pStrData[i];

		if (letter == '\0' || letter == '\n')
			break;

		if (letter == ' ' || letter == '\t')
			separator = i - *index;

		pFont->SetLetter(letter);
		nw = pFont->GetWidth();

		//Log("'%c' %f - %f", letter, nw, w+nw);
		if (w + nw > maxWidth)
			break;
		//Log(".");
		w += nw;
		len++;

		nw = pFont->GetSpacing();
		//Log("spacing %f - %f", nw, w+nw);
		if (w + nw > maxWidth)
			break;
		//Log(".");
		w += nw;
	}

	u32 ch = this->pStrData[*index + len];
	if (ch != '\0' && ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && len != 0 && separator != 0)
	{
		len = separator + 1;
		if (len > 0)
			w = this->GetWidth(*index, len);
	}

	*lineWidth = w;
	return len;
}

void Text::Draw(u32 index, u32 size)
{
	if (!pFont || !pStrData || !fWidth)
		return;

	u32 total = size;
	if (!total)
	{
		total = String::Length(pStrData);
	}
	else
	{
		ASSERT_MSG(index + total <= String::Length(pStrData), "Text index + size out of string bounds.");
	}

	//if (pScreen->HasModeChanged())
	//	fWidth = this->GetWidth(index, total);

	f32 fX = fPosX;
	f32 fY = fPosY;

	pFont->SetBlending(iOperation);
	pFont->SetColor(iColor);
	pFont->SetScale(fScaleX, fScaleY);
	for (u32 i = 0; i < total; i++)
	{
		u32 letter = pStrData[index + i];

		pFont->SetLetter(letter);
		pFont->SetPosition(fX, fY);
		pFont->Draw();

		fX += (pFont->GetWidth() + pFont->GetSpacing()) * fScaleX;
	}
}

/*
INLINE BOOL Text::IsChanged()
{
	return bChanged;
}

INLINE void Text::SetChanged(BOOL bChanged)
{
	this->bChanged = bChanged;
}
*/

INLINE f32 Text::GetFontTracking() const
{
	f32 ret = 0.0f;
	if (pFont)
		ret = pFont->GetTracking();

	return ret;
}

INLINE f32 Text::GetWidth() const
{
	return fWidth;
}

INLINE f32 Text::GetHeight() const
{
	return fHeight;
}

INLINE f32 Text::GetX() const
{
	return fPosX;
}

INLINE f32 Text::GetY() const
{
	return fPosY;
}

INLINE void Text::SetPosition(f32 x, f32 y)
{
	fPosX = x;
	fPosY = y;
}

INLINE void Text::SetFont(Font *f)
{
	if (this->pFont)
		pFont->Release();

	this->pFont = f;
	this->pFont->Acquire();

	this->fHeight = this->pFont->GetHeight();
	if (pStrData)
		this->fWidth = this->GetWidth(0, String::Length(pStrData));
}

INLINE void Text::SetBlending(IRenderable::eBlendMode op)
{
	this->iOperation = op;
}

INLINE void Text::SetColor(u8 r, u8 g, u8 b, u8 a)
{
	this->iColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void Text::SetColor(PIXEL px)
{
	this->iColor = px;
}

INLINE void Text::SetScale(f32 scaleX, f32 scaleY)
{
	if (scaleX == fScaleX && scaleY == fScaleY)
		return;

	fScaleX = scaleX;
	fScaleY = scaleY;
}

INLINE WideString Text::GetText() const
{
	return this->pStrData;
}


} // namespace
