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
class IFileSystem : public IModule, public IEventSystemListener
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
