/*! \file WiiMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief Memory management system for main ram
*/

#ifndef __WII_MEMORY_MANAGER_H__
#define __WII_MEMORY_MANAGER_H__

#include "Defines.h"

#ifdef _WII_

#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"

#include <stdlib.h>

namespace Seed { namespace WII {

class WiiMemoryPool : public IMemoryPool
{
	friend class MemoryManager;

	public:
		virtual void *Alloc(SIZE_T len, const char *desc, const char *owner)
		{
			UNUSED(desc)
			UNUSED(owner)
			return WiiAllocFromAllocator(&allocator, WiiRoundUp32B(len));
		}

		virtual void Free(void *ptr)
		{
			WiiFreeToAllocator(&allocator, ptr);
		}

		virtual u32 GetFreeMemory() const
		{
			return static_cast<u32>(WiiGetTotalFreeSizeForExpHeap(heapHandle));
		}

	protected:
		WiiHeapHandle heapHandle;
		WiiAllocator allocator;

		virtual void Initialize() = 0;
};

class DefaultMemoryPool : public WiiMemoryPool
{
	friend class MemoryManager;

	private:
		virtual void Initialize();
};

class LargeMemoryPool : public WiiMemoryPool
{
	friend class MemoryManager;

	private:
		virtual void Initialize();
};

class MemoryManager : public IMemoryManager
{
	public:
		virtual ~MemoryManager() {}

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual u32 GetFreeMemory() const;
		virtual void *Alloc(SIZE_T len, IMemoryPool *pool = &defaultPool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool = &defaultPool);

		virtual void Info();

		static MemoryManager instance;
		static DefaultMemoryPool defaultPool;
		static LargeMemoryPool largePool;
};

MemoryManager *const pMemoryManager = &MemoryManager::instance;

/// Default Pool for Memory Manager
DefaultMemoryPool *const pDefaultPool = &MemoryManager::defaultPool;

/// Memory Pool for Wii MEM2 memory
LargeMemoryPool *const pLargePool = &MemoryManager::largePool;

}} // namespace

#else // _WII_
	#error "Include 'MemoryManager.h' instead 'platform/wii/WiiMemoryManager.h' directly."
#endif // _WII_
#endif // __WII_MEMORY_MANAGER_H__
