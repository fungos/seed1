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

/*! \file StringCache.cpp
	\author	Danny Angelo Carminati Grein
	\brief String Cache class
*/


#include "StringCache.h"
#include "Log.h"
#include "File.h"
#include "Formats.h"

#define TAG	"[StringCache] "
#define STRINGCACHE_TABLE_FILE	"strlist.data"

namespace Seed {

SEED_SINGLETON_DEFINE(StringCache);

StringCache::StringCache()
	: stFile()
	, iStringCount(0)
	, pStringTable(NULL)
{
}

StringCache::~StringCache()
{
}

INLINE BOOL StringCache::Initialize()
{
	this->BuildStringTable();
	return TRUE;
}

INLINE BOOL StringCache::Shutdown()
{
	this->DestroyStringTable();
	return TRUE;
}

INLINE BOOL StringCache::Reset()
{
	return TRUE;
}

void StringCache::BuildStringTable()
{
	if (!stFile.GetData())
	{
		pFileSystem->Open(STRINGCACHE_TABLE_FILE, &stFile);
		//ASSERT_NULL(stFile.GetData());
		if (stFile.GetData())
		{
			const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
			ObjectHeader *block = NULL;
			READ_STRUCT(block, ObjectHeader, ptr);

			SECURITY_CHECK(seed_validate_block(&stFile, block, SST_OBJECT_MAGIC, SST_OBJECT_VERSION), "Invalid block header for string cache list.");
			READ_U32(iStringCount, ptr);

			pStringTable = (CachedString *)pMemoryManager->Alloc(iStringCount * sizeof(const char *));

			for (u32 s = 0; s < iStringCount; s++)
			{
				char *str = NULL;
				READ_STR(str, ptr);
				pStringTable[s] = str;
			}
		}
		else
		{
			Info(TAG "String cache %s file not found, this feature will be disabled.", STRINGCACHE_TABLE_FILE);
		}
	}
}

INLINE void StringCache::DestroyStringTable()
{
	pMemoryManager->Free(pStringTable);
	stFile.Close();
}

INLINE const char *StringCache::GetStringById(u32 strId) const
{
	const char *s = NULL;
	if (pStringTable)
		s = pStringTable[strId];

	return s;
}

INLINE u32 StringCache::GetIdByString(const char *str) const
{
	u32 r = SEED_INVALID_ID;
	if (pStringTable)
	{
		for (u32 s = 0; s < iStringCount; s++)
		{
			if (STRCMP(pStringTable[s], str) == 0)
			{
				r = s;
				break;
			}
		}
	}

	return r;
}

INLINE const char *StringCache::GetObjectName() const
{
	return "StringCache";
}

} // namespace
