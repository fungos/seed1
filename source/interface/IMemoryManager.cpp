/*! \file IMemoryManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Manager class interface
*/


#include "interface/IMemoryManager.h"

namespace Seed {


IMemoryManager::IMemoryManager()
{
}

IMemoryManager::~IMemoryManager()
{
}

INLINE u32 IMemoryManager::GetFreeMemory() const
{
	return 0;
}

INLINE void *IMemoryManager::Alloc(SIZE_T len, IMemoryPool *pool, const char *desc, const char *owner)
{
	UNUSED(len);
	UNUSED(pool);
	UNUSED(desc);
	UNUSED(owner);

	return NULL;
}

INLINE void IMemoryManager::Free(void *ptr, IMemoryPool *pool)
{
	UNUSED(ptr);
	UNUSED(pool);
}

INLINE void IMemoryManager::Info()
{
}


} // namespace
