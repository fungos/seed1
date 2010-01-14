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

StringCache StringCache::instance;

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
	if (pStringTable)
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

} // namespace
