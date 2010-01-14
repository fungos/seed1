/*! \file IMemoryPool.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Pool class interface
*/


#include "interface/IMemoryPool.h"

namespace Seed {


IMemoryPool::IMemoryPool()
	: iAllocations(0)
	, iTotalAllocations(0)
{
}

IMemoryPool::~IMemoryPool()
{
}

INLINE u32 IMemoryPool::GetFreeMemory() const
{
	return 0;
}

INLINE void IMemoryPool::Print() const
{
}

INLINE void *IMemoryPool::Alloc(SIZE_T len, const char *desc, const char *owner)
{
	UNUSED(len);
	UNUSED(desc);
	UNUSED(owner);

	return NULL;
}

INLINE void IMemoryPool::Free(void *ptr)
{
	UNUSED(ptr);
}


} // namespace

