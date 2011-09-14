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

/*! \file PcFileSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief FileSystem PC Implementation
*/

#include "FileSystem.h"

#if defined(_PC_)

#include "Log.h"
#include "MemoryManager.h"
#include "File.h"
#include "System.h"

#include <stdio.h>
#include <stdlib.h>

#include "platform/pc/platform.h"

#define TAG "[FileSystem] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(FileSystem)

FileSystem::FileSystem()
	: iLastLength(0)
{
	//this->SetWriteableDirectory("../savedata/");
}

FileSystem::~FileSystem()
{
}

BOOL FileSystem::Initialize()
{
	Log(TAG "Initializing...");
	IFileSystem::Initialize();

	if (!this->pWorkDir)
	{
		char outdir[1024];
		FilePath dir[1024];
		FilePath dir2[1024];
		memset(dir2, '\0', 1024);
		memset(dir, '\0', 1024);
		memset(outdir, '\0', 1024);
		get_current_directory(dir, 1024);

#if defined(WIN32)
		_snwprintf(dir2, 1024, L"%s\\%s\\", dir, FILESYSTEM_DEFAULT_PATH_WIDE);
		for (u32 i = 0; i < 1024; i++) // for print purpose only...
			outdir[i] = (char)dir[i];
#elif defined(_QT_)
		snprintf((char *)dir2, 1024, "%S/%s/", dir, FILESYSTEM_DEFAULT_PATH);
		memcpy(outdir, dir, 1024);
#else
		snprintf((char *)dir2, 1024, "%s/%s/", dir, FILESYSTEM_DEFAULT_PATH);
		memcpy(outdir, dir, 1024);
#endif

		Info(TAG "Working dir is: %s", outdir);

		if (!change_directory(dir2))
		{
			Log(TAG "Could not change current directory.");
		}
	}
	this->BuildFileTable();
	this->Reset();
	Log(TAG "Initialization completed. Free Memory: %d.", pMemoryManager->GetFreeMemory());

	return TRUE;
}

INLINE BOOL FileSystem::Reset()
{
	return TRUE;
}

INLINE BOOL FileSystem::Shutdown()
{
	this->DestroyFileTable();
	return this->Reset();
}

BOOL FileSystem::Open(const char *fname, File *file, IMemoryPool *pool)
{
	file->Close();

	BOOL result = IFileSystem::Open(fname, file, pool);
	if (!result)
	{
		file->pData = this->Read(fname, pool, &file->iSize);
		file->pName = fname;
		file->pPool = pool;
		file->bPackaged = FALSE;
		result = file->pData != NULL;
	}

	return result;
}

void *FileSystem::Read(const char *fname, IMemoryPool *pool, u32 *length)
{
	void *ret = NULL;
	FILE *fp = fopen(fname, "rb");

#if defined(WIN32)
	if (!fp)
		fp = _wfopen((wchar_t *)fname, L"rb");
#endif

	if (fp)
	{
		fseek(fp, 0L, SEEK_END);
		this->iLastLength = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		void *buff = pMemoryManager->Alloc(this->iLastLength, pool, fname, "File");
		ASSERT_MSG(buff != NULL, "ERROR: Out of memory - trying to load file.");

		u32 total = fread(buff, 1, iLastLength, fp);
		fclose(fp);

		if (total != this->iLastLength)
		{
			Log(TAG "WARNING: fread bytes read mismatch (diff: %d).", iLastLength - total);
			Log(TAG "WARNING: %s.", fname);
		}

		*length = this->iLastLength;

		ret = buff;
	}
	else
	{
		Log(TAG "WARNING: Could not find %s file.", fname);
	}

	return ret;
}

INLINE u32 FileSystem::GetLength() const
{
	return iLastLength;
}

INLINE void FileSystem::SetWorkDirectory(const FilePath *dir)
{
	IFileSystem::SetWorkDirectory(dir);
	if (change_directory(dir))
	{
		Log(TAG "Could not change current directory.");
	}
	else
	{
		Log(TAG "Changing working directory to: %s", dir);
	}
}

INLINE void FileSystem::MakeDirectory(const FilePath *dir) const
{
	create_directory(dir);
}

}} // namespace

#endif // _PC_
