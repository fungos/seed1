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

/*! \file WiiMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief Memory management system for main ram
*/

#ifndef __WII_MEMORY_MANAGER_H__
#define __WII_MEMORY_MANAGER_H__

#include "Defines.h"

#if defined(_WII_)

#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"
#include "Singleton.h"

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
	SEED_SINGLETON_DECLARE(MemoryManager);
	public:
		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual u32 GetFreeMemory() const;
		virtual void *Alloc(SIZE_T len, IMemoryPool *pool = &defaultPool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool = &defaultPool);

		virtual void Info();

		static DefaultMemoryPool defaultPool;
		static LargeMemoryPool largePool;

	private:
		SEED_DISABLE_COPY(MemoryManager);
};

extern "C" {

SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(MemoryManager);

/// Default Pool for Memory Manager
SEED_CORE_API extern DefaultMemoryPool *const pDefaultPool = &MemoryManager::defaultPool;

/// Memory Pool for Wii MEM2 memory
SEED_CORE_API extern LargeMemoryPool *const pLargePool = &MemoryManager::largePool;

}

}} // namespace

#else // _WII_
	#error "Include 'MemoryManager.h' instead 'platform/wii/WiiMemoryManager.h' directly."
#endif // _WII_
#endif // __WII_MEMORY_MANAGER_H__
