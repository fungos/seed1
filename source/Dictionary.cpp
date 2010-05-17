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

#include "Dictionary.h"
#include "FileSystem.h"
#include "Formats.h"
#include "System.h"

#define TAG	"[Dictionary] "

namespace Seed {

SEED_SINGLETON_DEFINE(Dictionary);

Dictionary::Dictionary()
	: pPool(NULL)
	, stFile()
	, iLang(Seed::en_US)
	, iTotalGlyphs(0)
	, iTotalStrings(0)
	, pGlyphs(NULL)
	, pStrings(NULL)
{
}

Dictionary::~Dictionary()
{
}

INLINE BOOL Dictionary::Initialize()
{
	BOOL ret = this->Reset();
	this->SetLanguage(pSystem->GetLanguage());
	pSystem->AddListener(this);

	return ret;
}

INLINE BOOL Dictionary::Shutdown()
{
	return this->Reset();
}

INLINE BOOL Dictionary::Reset()
{
	iTotalGlyphs	= 0;
	iTotalStrings	= 0;
	iLang			= Seed::en_US;
	pGlyphs			= NULL;
	pStrings		= NULL;
	pPool			= NULL;
	stFile.Close();

	return TRUE;
}

INLINE void Dictionary::SetLanguage(Seed::eLanguage lang, IMemoryPool *pool)
{
	// REQUIREMENT:
	/*
	The first file from filelist MUST BE strings.dict.
	This is because strings.dict is unique for each language, and we must know it's ID because
	currently we can only open "localized" files BY ID.

	A possible fix for this is that Dictionary can have an array of possible filenames to try and then open.
	*/
	this->Reset();
	this->pPool = pool;

	const char *file = _F(0);
	if (!file || !pFileSystem->Open(file, &stFile, pool))
	{
		Info(TAG, "strings.dict not found, skipping dictionary.");
		return;
	}

	const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
	ObjectHeader *block = NULL;
	READ_STRUCT(block, ObjectHeader, ptr);
	SECURITY_CHECK(seed_validate_block(&stFile, block, DICT_OBJECT_MAGIC, DICT_OBJECT_VERSION), "Invalid block header for dictionary.");

	u32 code = 0;
	READ_U32(code, ptr);
	ASSERT_MSG(static_cast<u32>(lang) == code, "Dictionary has an invalid language code.");
	this->iLang = lang;

	READ_U32(this->iTotalGlyphs, ptr);
	READ_U32(this->iTotalStrings, ptr);

	if (this->iTotalGlyphs)
		this->pGlyphs = reinterpret_cast<const u16 *>(ptr);

	if (this->iTotalStrings)
		this->pStrings = reinterpret_cast<const u16 *>(ptr + iTotalGlyphs + iTotalGlyphs);

	// we can alloc a table for strings using a structure {u32, u16 *}
	// and map them on load too but I will not do it. Or, I can do
	// this table inside the file and update the pointers (take care
	// of 32bit-64bit portability) on load.
}

INLINE const u16 *Dictionary::GetGlyphTable(u32 *outLen) const
{
	*outLen = this->iTotalGlyphs;
	return this->pGlyphs;
}

const u16 *Dictionary::GetString(u32 stringId, u32 *outLen) const
{
	ASSERT_MSG(stringId != SEED_INVALID_ID, "Error: stringId is invalid. Rebuild your assets.");
	const void *ret = NULL;

	if (stringId < this->iTotalStrings)
	{
		u32 len = 0;
		const u8 *ptr = reinterpret_cast<const u8 *>(this->pStrings);
		READ_U32(len, ptr);
		for (u32 i = 0; i < stringId; i++)
		{
			ptr += ((len + 1) * sizeof(u16));
			READ_U32(len, ptr);
		}

		if (outLen)
			*outLen = len;

		ret = reinterpret_cast<const void *>(ptr);
	}

	return (const WideString)ret;
}

INLINE u32 Dictionary::GetSize() const
{
	return this->iTotalStrings;
}

INLINE void Dictionary::OnSystemLanguageChanged(const EventSystem *ev)
{
	this->SetLanguage(ev->GetLanguageNew(), pPool);
}

INLINE const char *Dictionary::GetObjectName() const
{
	return "Dictionary";
}

INLINE int Dictionary::GetObjectType() const
{
	return Seed::ObjectDictionary;
}

} // namespace
