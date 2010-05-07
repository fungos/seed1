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

/*! \file IFileSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the File System class interface
*/

#include "interface/IFileSystem.h"
#include "interface/IEventFileSystemListener.h"
#include "Log.h"
#include "PackageManager.h"
#include "File.h"
#include "Formats.h"
#include "System.h"
#include "EventSystem.h"

#define TAG	"[IFileSystem] "
#define FILESYSTEM_TABLE_FILE	"filelist.data"

namespace Seed {

IFileSystem::IFileSystem()
	: pWorkDir(NULL)
	, pWriteDir(NULL)
	, vListeners()
	, pFile(NULL)
	, iLangCount(0)
	, iFileCount(0)
	, pLangTable()
	, pFileTable(NULL)
	, pCurrentTable(NULL)
	, pOldTable(NULL)
{
}

IFileSystem::~IFileSystem()
{
}

INLINE BOOL IFileSystem::Initialize()
{
	// If user haven't setup a write directory, we assume one
	if (!pWriteDir)
		this->SetWriteableDirectory(pSystem->GetHomeFolder());

	return TRUE;
}

void IFileSystem::BuildFileTable()
{
	if (!pFile)
	{
		pFile = New(File(FILESYSTEM_TABLE_FILE));
		//ASSERT_NULL(pFile->pData);
		if (pFile->pData)
		{
			const u8 *ptr = static_cast<const u8 *>(pFile->pData);
			ObjectHeader *block = NULL;
			READ_STRUCT(block, ObjectHeader, ptr);

			SECURITY_CHECK(seed_validate_block(pFile, block, FST_OBJECT_MAGIC, FST_OBJECT_VERSION), "Invalid block header for filelist.");
			READ_U32(iLangCount, ptr);
			READ_U32(iFileCount, ptr);

			pFileTable = (StringFile *)pMemoryManager->Alloc(iLangCount * iFileCount * sizeof(const char *));
			for (u32 l = 0; l < iLangCount; l++)
			{
				u32 lang = 0;
				READ_U32(lang, ptr);
				pLangTable[lang] = &pFileTable[l * iFileCount];

				for (u32 s = 0; s < iFileCount; s++)
				{
					char *str = NULL;
					READ_STR(str, ptr);
					pFileTable[(l*iFileCount)+s] = str;
				}
			}

			pOldTable = pCurrentTable = pLangTable[pSystem->GetLanguage()];
			pSystem->AddListener(this);
		}
		else
		{
			Info(TAG "Could not find %s, not using localization file table and file by id feature.", FILESYSTEM_TABLE_FILE);
		}
	}
}

void IFileSystem::DestroyFileTable()
{
	Delete(pFile);
	pMemoryManager->Free(pFileTable);

	pSystem->RemoveListener(this);
}

INLINE const char *IFileSystem::GetFileNameById(u32 fileId) const
{
	const char *ret = NULL;
	if (pFileTable)
	{
		ASSERT_MSG(fileId != SEED_INVALID_ID, "Error: fileId is invalid. Rebuild your assets.");
		ret = pCurrentTable[fileId];
	}

	return ret;
}

INLINE u32 IFileSystem::GetIdByFileName(const char *filename) const
{
	u32 r = SEED_INVALID_ID;
	if (pFileTable)
	{
		for (u32 s = 0; s < iFileCount; s++)
		{
			if (STRCMP(pCurrentTable[s], filename) == 0)
			{
				r = s;
				break;
			}
		}
	}

	return r;
}

INLINE void IFileSystem::OnSystemLanguageChanged(const EventSystem *ev)
{
	if (pFileTable)
	{
		this->pCurrentTable = pLangTable[ev->GetLanguageNew()];
		this->pOldTable = pLangTable[ev->GetLanguageCurrent()];
	}
}

INLINE BOOL IFileSystem::OpenById(u32 fileId, File *file, IMemoryPool *pool)
{
	return this->Open(this->GetFileNameById(fileId), file, pool);
}

BOOL IFileSystem::Open(const char *fileName, File *file, IMemoryPool *pool)
{
	ASSERT_NULL(fileName);
	ASSERT_NULL(file);
	ASSERT_NULL(pool);

	BOOL ret = FALSE;

	file->pName = NULL;
	file->pData = NULL;
	file->pPool = NULL;
	file->iSize = 0;
	file->bPackaged = FALSE;

	if (pPackageManager->IsInitialized())
	{
		file->pData = pPackageManager->GetFile(fileName, &file->iSize);

		if (file->pData)
		{
			ret = TRUE;
			file->pName = fileName;
			file->pPool = pool;
			file->bPackaged = TRUE;
		}
	}

	return ret;
}

INLINE void IFileSystem::Close(File *file)
{
	file->Close();
}

INLINE void IFileSystem::AddListener(IEventFileSystemListener *listener)
{
	ASSERT_NULL(listener);
	vListeners.push_back(listener);
}

INLINE void IFileSystem::RemoveListener(IEventFileSystemListener *listener)
{
	ASSERT_NULL(listener);
	ListenerIterator p = std::find(vListeners.begin(), vListeners.end(), listener);

	if (p != vListeners.end())
		vListeners.erase(p);

	ListenerVector(vListeners).swap(vListeners);
}

INLINE void IFileSystem::SendEventFileSystemFatalError(const EventFileSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventFileSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnFileSystemFatalError(ev);
	}
}

INLINE void IFileSystem::SendEventFileSystemWrongDiskError(const EventFileSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventFileSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnFileSystemWrongDiskError(ev);
	}
}

INLINE void IFileSystem::SendEventFileSystemNoDiskError(const EventFileSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventFileSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnFileSystemNoDiskError(ev);
	}
}

INLINE void IFileSystem::SendEventFileSystemRetryError(const EventFileSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventFileSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnFileSystemRetryError(ev);
	}
}

INLINE void IFileSystem::SendEventFileSystemOperationResumed(const EventFileSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventFileSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnFileSystemOperationResumed(ev);
	}
}

INLINE void IFileSystem::SendEventFileSystemLoadCompleted(const EventFileSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventFileSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnFileSystemLoadCompleted(ev);
	}
}

INLINE void IFileSystem::SetWorkDirectory(const char *dir)
{
	pWorkDir = dir;
}

INLINE const char *IFileSystem::GetWorkDirectory() const
{
	return pWorkDir;
}

INLINE void IFileSystem::SetWriteableDirectory(const char *dir)
{
	pWriteDir = dir;
}

INLINE const char *IFileSystem::GetWriteableDirectory() const
{
	return pWriteDir;
}

INLINE void IFileSystem::MakeDirectory(const char *dir) const
{
	UNUSED(dir);
	SEED_ABSTRACT_METHOD;
}

INLINE BOOL IFileSystem::IsRequired() const
{
	return TRUE;
}

INLINE const char *IFileSystem::GetObjectName() const
{
	return "FileSystem";
}

} // namespace
