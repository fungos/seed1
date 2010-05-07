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
#include <sys/types.h>

#define TAG "[MemoryManager] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(MemoryManager);

DefaultMemoryPool MemoryManager::defaultPool(MB60);
DefaultMemoryPool *const pDefaultPool = &MemoryManager::defaultPool;

LargeMemoryPool MemoryManager::largePool(MB20);
LargeMemoryPool *const pLargePool = &MemoryManager::largePool;

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
	defaultPool.Initialize();
	largePool.Initialize();

	Log(TAG "Initialization completed. Free Memory: %d.", this->GetFreeMemory());

	this->Info();

	return r;
}

BOOL MemoryManager::Shutdown()
{
	Log(TAG "Terminating...");
	BOOL r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

INLINE u32 MemoryManager::GetFreeMemory()
{
	return defaultPool.GetFreeMemory() + largePool.GetFreeMemory();
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
	Log(TAG "Default Pool: %d (Total allocations: %d Current: %d)", defaultPool.GetFreeMemory(), defaultPool.iTotalAllocations, defaultPool.iAllocations);
	Log(TAG "Large Pool..: %d (Total allocations: %d Current: %d)", largePool.GetFreeMemory(), largePool.iTotalAllocations, largePool.iAllocations);
}

}} // namespace

using namespace Seed::PC;

/*
FIXME: 2009-03-04 | BUG | PC | Something related to STL breaks when we use a memory pool here. | Danny Angelo Carminati Grein

Algo faz com que a STL (incerto) quebre quando se utiliza memory pool aqui.
Nao faz muita diferenca isso agora, mas seria importante ter isso aqui para poder ter um mapa melhor do consumo de memoria.
Alem da STL, cada new / delete no codigo nao sera mapeado pelo memory manager tambem.
Teremos uma visao aproximada do consumo de memoria mas nao exata. Isso pode afetar o desenvolvimento multiplataforma, exemplo: PC -> WII mas nao WII -> PC
*/
void *operator new(SIZE_T size) throw()
{
	//return pLargePool->Alloc(size, "Code", "Auto");
	return malloc(size);
}

void *operator new[](SIZE_T size) throw()
{
	//return pLargePool->Alloc(size, "Code", "Auto");
	return malloc(size);
}

//overwrites the delete operator
void operator delete(void * p) throw()
{
	//pLargePool->Free(p);
	return free(p);
}

void operator delete[](void * p) throw()
{
	//pLargePool->Free(p);
	return free(p);
}

#endif // _PC_
