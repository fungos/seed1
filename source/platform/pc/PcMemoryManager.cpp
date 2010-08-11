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

/*! \file PcMemoryManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager PC Implementation
*/

#include "MemoryManager.h"

#if defined(_PC_)

#include "Log.h"
#include "Enum.h"
#include "SeedInit.h"
#include <sys/types.h>

#define TAG "[MemoryManager] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(MemoryManager);

extern "C" 
{
PcMemoryPool *pDefaultPool = NULL;
PcMemoryPool *pLargePool = NULL;
}

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
}

BOOL MemoryManager::Reset()
{
	return TRUE;
}

BOOL MemoryManager::Initialize()
{
	Log(TAG "Initializing...");

	BOOL r = this->Reset();
	ePlatformSimulation plat = pConfiguration->GetPlatformSimulation();
	
	switch (plat)
	{
		case Seed::SimulateIOS3G:
		{
			pLargePool = pDefaultPool = static_cast<PcMemoryPool *>(this->CreatePool(MB10, "iphone"));
		}
		break;
		
		default:
		{
			pDefaultPool = static_cast<PcMemoryPool *>(this->CreatePool(MB20, "default"));
			pLargePool = static_cast<PcMemoryPool *>(this->CreatePool(MB60, "large"));
		}
		break;
	};

	Log(TAG "Initialization completed. Free Memory: %d.", this->GetFreeMemory());

	this->Info();

	return r;
}

BOOL MemoryManager::Shutdown()
{
	Log(TAG "Terminating...");
	
	for (u32 i = 0; i < arPool.Size(); i++)
	{
		IMemoryPool *pool = arPool[i];
		Log(TAG "Releasing pool %s.", pool->pcName);
		Delete(pool);
	}
	arPool.Truncate();
	
	BOOL r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

INLINE IMemoryPool *MemoryManager::CreatePool(u32 len, const char *name)
{
	PcMemoryPool *pool = New(PcMemoryPool(len, name));
	pool->Initialize();
	
	arPool.Add(pool);
	
	return pool;
}

INLINE u32 MemoryManager::GetFreeMemory() const
{
	u32 free = 0;
	
	for (u32 i = 0; i < arPool.Size(); i++)
	{
		IMemoryPool *pool = arPool[i];
		free += pool->GetFreeMemory();
	}

	return free;
}

INLINE void *MemoryManager::Alloc(SIZE_T len, IMemoryPool *pool, const char *desc, const char *owner)
{
	return pool->Alloc(ROUND_UP(len, 32), desc, owner);
}

INLINE void MemoryManager::Free(void *ptr, IMemoryPool *pool)
{
	if (ptr)
		pool->Free(ptr);
	ptr = NULL;
}

INLINE void MemoryManager::Info()
{
	u32 free = 0;
	for (u32 i = 0; i < arPool.Size(); i++)
	{
		IMemoryPool *pool = arPool[i];
		Log(TAG "Pool %s: %d (Total allocations: %d Current: %d)", pool->pcName, pool->GetFreeMemory(), pool->iTotalAllocations, pool->iAllocations);
		free += pool->GetFreeMemory();
	}
	Log(TAG "Total free memory: %d", free);
}

}} // namespace

using namespace Seed::PC;

#endif // _PC_
