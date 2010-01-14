/*! \file IphFileSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief FileSystem Iphone Implementation
*/

#ifdef _IPHONE_

#include "Defines.h"
#include "FileSystem.h"
#include "Log.h"
#include "MemoryManager.h"
#include "IphoneView.h"
#include "File.h"

#include <stdio.h>
#include <stdlib.h>


#define TAG "[FileSystem] "


namespace Seed { namespace iPhone {


FileSystem FileSystem::instance;


FileSystem::FileSystem()
	: iLastLength(0)
{
}


FileSystem::~FileSystem()
{
}


BOOL FileSystem::Initialize()
{
	Log(TAG "Initializing...");

	char dir[1024];

	snprintf(dir, 1024, "%s/%s", iphGetRootPath(), FILESYSTEM_DEFAULT_PATH);
	Log(TAG "%s", dir);

	chdir(dir);

	this->Reset();
	Log(TAG "Initialization completed. Free Memory: %d.", pMemoryManager->GetFreeMemory());

	return TRUE;
}


BOOL FileSystem::Reset()
{
	return TRUE;
}


BOOL FileSystem::Shutdown()
{
	return this->Reset();
}


BOOL FileSystem::Open(const char *fname, File *file, IMemoryPool *pool)
{
	ASSERT_NULL(fname);
	ASSERT_NULL(file);
	ASSERT_NULL(pool);

	BOOL ret = FALSE;

	FILE *fp = fopen(fname, "rb");
	if (fp)
	{
		fseek(fp, 0L, SEEK_END);
		this->iLastLength = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		if (iLastLength)
		{
			void *buff = pMemoryManager->Alloc(this->iLastLength, pDefaultPool, "File", "FileSystem");
			ASSERT_NULL(buff);

			u32 total = fread(buff, 1, this->iLastLength, fp);
			fclose(fp);
			
			if (total != this->iLastLength)
			{
				Log(TAG "WARNING: fread bytes read mismatch (diff: %d).", iLastLength - total);
				Log(TAG "WARNING: %s.", fname);			
			}

			file->iSize = this->iLastLength;
			file->pData = buff;
			file->pPool = pool;

			ret = TRUE;
		}
		else
		{
			Log(TAG "WARNING: Empty existing file: %s.", fname);
		}
	}
	else
	{
		Log(TAG "WARNING: Could not find %s file.", fname);
	}

	return ret;
}


}} // namespace

#endif // _IPHONE_

