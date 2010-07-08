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

/*! \file Font.cpp
	\author	Danny Angelo Carminati Grein
	\brief A font class
*/

#include "Font.h"
#include "Formats.h"
#include "Dictionary.h"
#include "System.h"
#include "Texture.h"
#include "Number.h"

#define FONT_OFFSET		33
#define FONT_TOTAL		90

namespace Seed {

IResource *FontResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Font *fnt = New(Font());
	fnt->Load(filename, res, pool);

	return fnt;
}

Font::Font()
	: stFile()
	, mFont()
	, mFontExt()
	, pFont(NULL)
	, fSpacing(0)
	, fTracking(0)
	, fSpaceWidth(0)
	, fWidth(0)
	, fHeight(0)
	, iCurrent(0)
	, iTotalGlyphs(0)
	, pGlyphs(NULL)
	, bSpace(FALSE)
{
}

Font::~Font()
{
	this->Unload();
}

BOOL Font::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(res);
	ASSERT_NULL(pool);

	if (this->Unload())
	{
		this->pFilename = filename;
		this->pRes = res;
		this->pPool = pool;

		stFile.Close();
		SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Could not open font file.");
		const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
		ObjectHeader *block = NULL;
		READ_STRUCT(block, ObjectHeader, ptr);
		SECURITY_CHECK(seed_validate_block(&stFile, block, FONT_OBJECT_MAGIC, FONT_OBJECT_VERSION), "Invalid block header for font.");

		READ_F32(this->fTracking, ptr);
		READ_F32(this->fSpacing, ptr);
		READ_F32(this->fSpaceWidth, ptr);

	/*
		const char *file = NULL;
		READ_STR(file, ptr);
		ASSERT_NULL(file);

		const char *ext = NULL;
		READ_STR(ext, ptr);
	*/
		u32 file = 0;
		READ_U32(file, ptr);

		u32 ext = 0;
		READ_U32(ext, ptr);

		if (ext != SEED_INVALID_ID /*&& pSystem->GetLanguage() != Seed::en_US*/)
		{
			this->mFontExt.Load(_F(ext), res, pool);
			this->pGlyphs = pDictionary->GetGlyphTable(&this->iTotalGlyphs);
		}

		this->mFont.Load(_F(file), res, pool);
		this->fHeight	= mFont.GetHeight();
		this->fWidth	= mFont.GetWidth();

		this->bLoaded	= TRUE;
	}

	return bLoaded;
}

BOOL Font::Unload()
{
	stFile.Close();
	this->mFont.Unload();
	this->mFontExt.Unload();
	this->pFont			= NULL;
	this->iCurrent		= 0;
	this->fSpacing		= 0;
	this->fTracking		= 0;
	this->fSpaceWidth	= 0;
	this->fWidth		= 0;
	this->fHeight		= 0;
	this->iTotalGlyphs	= 0;
	this->pGlyphs		= NULL;
	this->bLoaded		= FALSE;

	return TRUE;
}

u32 Font::SelectLetter(u32 letter)
{
	u32 nc = 0;
	if (letter > FONT_OFFSET - 1 && letter < FONT_TOTAL + FONT_OFFSET)
	{
		nc = letter - FONT_OFFSET;
		pFont = &mFont;
	}
	else
	{
		for (u32 i = 0; i < this->iTotalGlyphs; i++)
		{
			if (this->pGlyphs[i] == static_cast<u16>(letter))
			{
				pFont = &mFontExt;
				nc = i;
				break;
			}
		}
	}

	pFont->SetCurrentFrame(nc);
	pFont->SetLocalPosition(-pFont->fWidth / 2.0f, -pFont->fHeight / 2.0f);

	return nc;
}

const void *Font::GetLetter(u32 letter)
{
	this->SelectLetter(letter);
	return pFont->GetData();
}

void Font::SetLetter(u32 letter)
{
	if (letter == '\t')
	{
		this->bSpace = TRUE;
		this->fWidth = this->fSpaceWidth * 2 * Number::Abs(mFont.GetScaleX());
	}
	else if (letter == '\n')
	{
		this->bSpace = TRUE;
		this->fWidth = 0.0f;
	}
	else if (letter == ' ')
	{
		this->bSpace = TRUE;
		this->fWidth = this->fSpaceWidth * Number::Abs(mFont.GetScaleX());
	}
	else
	{
		this->bSpace = FALSE;
		this->SelectLetter(letter);
		this->fWidth = pFont->GetWidth();
	}
}

INLINE f32 Font::GetWidth() const
{
	return this->fWidth;
}

INLINE f32 Font::GetHeight() const
{
	return mFont.GetHeight();
}

INLINE void Font::SetSpacing(f32 spacing)
{
	this->fSpacing = spacing;
}

INLINE f32 Font::GetSpacing() const
{
	return this->fSpacing * Number::Abs(mFont.GetScaleX());;
}

INLINE void Font::SetSpaceWidth(f32 width)
{
	this->fSpaceWidth = width;
}

INLINE f32 Font::GetSpaceWidth() const
{
	return this->fSpaceWidth * Number::Abs(mFont.GetScaleX());
}

INLINE void Font::SetTracking(f32 tracking)
{
	this->fTracking = tracking;
}

INLINE f32 Font::GetTracking()
{
	return this->fTracking * Number::Abs(mFont.GetScaleX());
}

INLINE void Font::SetPosition(f32 x, f32 y)
{
	mFont.SetPosition(x, y);
	mFontExt.SetPosition(x, y);
}

INLINE void Font::SetLocalPosition(f32 x, f32 y)
{
	mFont.SetLocalPosition(x, y);
	mFontExt.SetLocalPosition(x, y);
}

INLINE u32 Font::GetUsedMemory() const
{
	return 0;
}

INLINE int Font::GetObjectType() const
{
	return Seed::ObjectFont;
}

INLINE const char *Font::GetObjectName() const
{
	return "Font";
}

INLINE void Font::SetBlending(eBlendMode op)
{
	mFont.SetBlending(op);
	mFontExt.SetBlending(op);
}

INLINE void Font::SetColor(PIXEL px)
{
	mFont.SetColor(px);
	mFontExt.SetColor(px);
}

INLINE void Font::Draw()
{
	if (this->bSpace)
		return;

	// TODO: remove delta
	pFont->Update(0.0f);
	pFont->Render();
}

INLINE void Font::Update(f32 dt)
{
	mFont.Update(dt);
	mFontExt.Update(dt);
}

INLINE void Font::SetScale(f32 x, f32 y)
{
	mFont.SetScale(x, y);
	mFontExt.SetScale(x, y);
}

INLINE void Font::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	ITexture *img = mFont.GetTexture();
	if (img)
		img->SetFilter(type, filter);

	img = mFontExt.GetTexture();
	if (img)
		img->SetFilter(type, filter);
}

SEED_DISABLE_INSTANCING_IMPL(Font);

} // namespace
