/*! \file PcMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager PC Implementation
*/

#ifndef __PC_MEMORY_MANAGER_H__
#define __PC_MEMORY_MANAGER_H__

#include "Defines.h"

#if defined(_PC_)

#include <stdlib.h>
#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"
#include "MemoryPool.h"

#define MB60	(u32)(59 * 1024 * 1024)
#define MB20	(u32)(19 * 1024 * 1024)


namespace Seed { namespace PC {


class MemoryManager : public IMemoryManager
{
	public:
		MemoryManager();
		virtual ~MemoryManager();

		virtual u32 GetFreeMemory();

		virtual void *Alloc(SIZE_T len, IMemoryPool *pool = &defaultPool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool = &defaultPool);
		virtual void Info();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static MemoryManager instance;
		static DefaultMemoryPool defaultPool;
		static LargeMemoryPool largePool;

	private:
		SEED_DISABLE_COPY(MemoryManager);
};

MemoryManager *const pMemoryManager = &MemoryManager::instance;
DefaultMemoryPool *const pDefaultPool = &MemoryManager::defaultPool;
LargeMemoryPool *const pLargePool = &MemoryManager::largePool;


}} // namespace


#else // _PC_

	#error "Include 'MemoryManager.h' instead 'platform/pc/PcMemoryManager.h' directly."

#endif // _PC_
#endif // __PC_MEMORY_MANAGER_H__
