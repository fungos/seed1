/*! \file PcFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief FileSystem PC Implementation
*/

#ifndef __PC_FILESYSTEM_H__
#define __PC_FILESYSTEM_H__

#include "Defines.h"

#if defined(_PC_)

#include "MemoryManager.h"
#include "interface/IFileSystem.h"

#define FILESYSTEM_DEFAULT_PATH	"data/"


namespace Seed { namespace PC {


class FileSystem : public IFileSystem
{
	public:
		FileSystem();
		virtual ~FileSystem();

		virtual BOOL Open(const char *pFname, File *file, IMemoryPool *pool = pDefaultPool);
		virtual u32 GetLength() const;
		virtual void MakeDirectory(const char *dir) const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual void SetWorkDirectory(const char *dir);

	public:
		static FileSystem instance;

	private:
		SEED_DISABLE_COPY(FileSystem);

		void *Read(const char *fname, IMemoryPool *pool, u32 *pLength);

	private:
		u32 iLastLength;
};

FileSystem *const pFileSystem = &FileSystem::instance;


}} // namespace


#else // _PC_

	#error "Include 'FileSystem.h' instead 'platform/pc/PcFileSystem.h' directly."

#endif // _PC_
#endif // __PC_FILESYSTEM_H__

