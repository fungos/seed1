/*! \file PcMemoryPool.cpp
	\author	Danny Angelo Carminati Grein
	\brief Memory Pool PC implementation
*/


#include "MemoryPool.h"

#ifdef _PC_


#ifdef YMEM_DEBUG
	#include "extra/ymanager/yallocwrap.h"

	#define SEED_MALLOC(len, desc, owner)	(void *)yalloc(this->pMgr, len, desc, owner)
	#define SEED_FREE(ptr)					yfree(this->pMgr, ptr)
	#define YMANAGER_CREATE(len)			this->pMgr = ycreate(len)
	#define YMANAGER_DESTROY				ydestroy(this->pMgr); this->pMgr = NULL;
	#define YMANAGER_GETFREEMEM				if (this->pMgr) return this->pMgr->GetFreeMemory(); else return 0;
	#define YMANAGER_PRINT					this->pMgr->PrintSnapshot();
#else
	#define SEED_MALLOC(len, desc, owner)	(void *)malloc(len)
	#define SEED_FREE(ptr)					free(ptr)
	#define YMANAGER_CREATE(len)
	#define YMANAGER_DESTROY
	#define YMANAGER_GETFREEMEM				return 0;
	#define YMANAGER_PRINT
#endif // DEBUG


namespace Seed { namespace PC {


PcMemoryPool::PcMemoryPool(u32 size)
	YMANAGER_INIT
{
	UNUSED(size);
	YMANAGER_CREATE(size);
}

PcMemoryPool::~PcMemoryPool()
{
	YMANAGER_DESTROY;
}

INLINE void PcMemoryPool::Initialize()
{
}

INLINE u32 PcMemoryPool::GetFreeMemory() const
{
	YMANAGER_GETFREEMEM;
	//return 0;
}

INLINE void PcMemoryPool::Print() const
{
	YMANAGER_PRINT;
}

void *PcMemoryPool::Alloc(SIZE_T len, const char *desc, const char *owner)
{
	UNUSED(owner);
	UNUSED(desc);
	iAllocations++;
	iTotalAllocations++;
	return SEED_MALLOC((unsigned long)len, desc, owner);
	//return NULL;
}

void PcMemoryPool::Free(void *ptr)
{
	iAllocations--;
	SEED_FREE(ptr);
}

DefaultMemoryPool::DefaultMemoryPool(u32 size)
	: PcMemoryPool(size)
{
}

DefaultMemoryPool::~DefaultMemoryPool()
{
}

LargeMemoryPool::LargeMemoryPool(u32 size)
	: PcMemoryPool(size)
{
}

LargeMemoryPool::~LargeMemoryPool()
{
}


}} // namespace


#endif // _PC_
