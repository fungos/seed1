/*! \file IMemoryPool.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Pool class interface
*/

#include "interface/IMemoryPool.h"
#include "Log.h"

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
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE void IMemoryPool::Print() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void *IMemoryPool::Alloc(SIZE_T len, const char *desc, const char *owner)
{
	UNUSED(len);
	UNUSED(desc);
	UNUSED(owner);
	SEED_ABSTRACT_METHOD;

	return NULL;
}

INLINE void IMemoryPool::Free(void *ptr)
{
	UNUSED(ptr);
	SEED_ABSTRACT_METHOD;
}

} // namespace
