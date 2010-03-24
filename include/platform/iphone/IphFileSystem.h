/*! \file IphFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief FileSystem Iphone Implementation
*/

#ifndef __IPH_FILESYSTEM_H__
#define __IPH_FILESYSTEM_H__

#ifdef _IPHONE_

#include "MemoryManager.h"
#include "interface/IFileSystem.h"

#define FILESYSTEM_DEFAULT_PATH	"data/"

namespace Seed { namespace iPhone {

class FileSystem : public IFileSystem
{
	public:
		FileSystem();
		virtual ~FileSystem();

		virtual BOOL Open(const char *pFname, File *file, IMemoryPool *pool = pDefaultPool);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static FileSystem instance;

	private:
		SEED_DISABLE_COPY(FileSystem);

	private:
		u32 iLastLength;
};

FileSystem *const pFileSystem = &FileSystem::instance;

}} // namespace

#else // _IPHONE_
	#error "Include 'FileSystem.h' instead 'platform/iphone/IphFileSystem.h' directly."
#endif // _IPHONE_
#endif // __IPH_FILESYSTEM_H__
