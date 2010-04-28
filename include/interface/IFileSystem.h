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

/*! \file IFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the File System class interface
*/

#ifndef __IFILESYSTEM_H__
#define __IFILESYSTEM_H__

#include "IModule.h"
#include "MemoryManager.h"
#include "Base.h"
#include "Enum.h"
#include "interface/IEventSystemListener.h"

#include <vector>
#include <algorithm>

#define _F(x)	pFileSystem->GetFileNameById(x)

namespace Seed {

class File;
class EventFileSystem;
class IMemoryPool;
class PackageManager;
class IEventFileSystemListener;

/// FileSystem Interface
/**
Any FileSystem implementation must implement this interface.
*/
class SEED_CORE_API IFileSystem : public IModule, public IEventSystemListener
{
	typedef std::vector<IEventFileSystemListener *> 	ListenerVector;
	typedef ListenerVector::iterator			  		ListenerIterator;

	public:
		IFileSystem();
		virtual ~IFileSystem();

		virtual BOOL Open(const char *fileName, File *file, IMemoryPool *pool = pDefaultPool);
		virtual void Close(File *file);

		virtual void SetWorkDirectory(const char *dir);
		virtual void SetWriteableDirectory(const char *dir);
		virtual void OnSystemLanguageChanged(const EventSystem *ev);
		virtual void MakeDirectory(const char *dir) const;

		const char *GetWorkDirectory() const;
		const char *GetWriteableDirectory() const;

		u32 GetIdByFileName(const char *filename) const;
		const char *GetFileNameById(u32 fileId) const;
		BOOL OpenById(u32 fileId, File *file, IMemoryPool *pool = pDefaultPool);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

		/// Listeners pattern methods
		void AddListener(IEventFileSystemListener *listener);
		void RemoveListener(IEventFileSystemListener *listener);

	protected:
		const char *pWorkDir;
		const char *pWriteDir;
		ListenerVector vListeners;

	protected:
		void SendEventFileSystemFatalError(const EventFileSystem *ev);
		void SendEventFileSystemWrongDiskError(const EventFileSystem *ev);
		void SendEventFileSystemNoDiskError(const EventFileSystem *ev);
		void SendEventFileSystemRetryError(const EventFileSystem *ev);
		void SendEventFileSystemOperationResumed(const EventFileSystem *ev);
		void SendEventFileSystemLoadCompleted(const EventFileSystem *ev);
		void BuildFileTable();
		void DestroyFileTable();

	private:
		SEED_DISABLE_COPY(IFileSystem);

	private:
		File	*pFile;
		u32		iLangCount;
		u32		iFileCount;

		typedef char* StringFile;
		StringFile	*pLangTable[Seed::MaximumLanguage];
		StringFile	*pFileTable;
		StringFile	*pCurrentTable;
		StringFile	*pOldTable;
};

} // namespace

#endif // __IFILESYSTEM_H__
