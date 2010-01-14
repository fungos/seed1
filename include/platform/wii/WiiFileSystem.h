/*! \file WiiFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief File system initialization
*/


#ifndef __WII_FILESYSTEM_H__
#define __WII_FILESYSTEM_H__

#if defined(_WII_)


#include "interface/IFileSystem.h"
#include "MemoryManager.h"

#define FILESYSTEM_DEFAULT_PATH	"/data/"
#define FILESYSTEM_DEFAULT_PACK "data.arc"


namespace Seed { namespace WII {


class FileSystem : public IFileSystem
{
	public:
		FileSystem();
		virtual ~FileSystem();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual BOOL Open(const char *pFname, File *file, IMemoryPool *pool = pDefaultPool);
		//virtual void *Alloc(const char *pFname, u32 *fileLen, IMemoryPool *pool = pDefaultPool);


	public:
		static FileSystem instance;


	private:
		BOOL ChangeDir(const char *dirName);
		void *Read(const char *fname, IMemoryPool *pool, u32 *pLength);
		void *Read(s32 entry, IMemoryPool *pool);


	private:
		s32 entryNumber;
		void *packageStart;
		BOOL bUsePackage;
		WiiHandle package;
		u32 lastLength;
};


FileSystem *const pFileSystem = &FileSystem::instance;


}} // namespace


#else // _WII_

	#error "Include 'FileSystem.h' instead 'platform/wii/WiiFileSystem.h' directly."

#endif // _WII_
#endif // __WII_FILESYSTEM_H__

