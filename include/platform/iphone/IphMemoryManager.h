/*! \file IphMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager Iphone Implementation
*/

#ifndef __IPH_MEMORY_MANAGER_H__
#define __IPH_MEMORY_MANAGER_H__

#ifdef _IPHONE_

#include <stdlib.h>
#include "interface/IMemoryManager.h"
#include "platform/iphone/IphMemoryPool.h"

namespace Seed { namespace iPhone {

class MemoryManager : public IMemoryManager
{
	public:
		MemoryManager();
		virtual ~MemoryManager();

		virtual u32 GetFreeMemory() const;

		virtual void *Alloc(SIZE_T len, IMemoryPool *pool = &defaultPool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool = &defaultPool);
		virtual void Info();

		//virtual u32 GetFreeMemory() const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static MemoryManager instance;
		static IphoneMemoryPool defaultPool;

	private:
		SEED_DISABLE_COPY(MemoryManager);
};

MemoryManager *const pMemoryManager = &MemoryManager::instance;
IphoneMemoryPool *const pDefaultPool = &MemoryManager::defaultPool;
#define pLargePool pDefaultPool

}} // namespace

#else // _IPHONE_
	#error "Include 'MemoryManager.h' instead 'platform/iphone/IphMemoryManager.h' directly."
#endif // _IPHONE_
#endif // __MEMORY_MANAGER__
