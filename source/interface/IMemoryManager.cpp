/*! \file IMemoryManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Manager class interface
*/

#include "interface/IMemoryManager.h"
#include "Log.h"

namespace Seed {

IMemoryManager::IMemoryManager()
{
}

IMemoryManager::~IMemoryManager()
{
}

INLINE u32 IMemoryManager::GetFreeMemory() const
{
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE void *IMemoryManager::Alloc(SIZE_T len, IMemoryPool *pool, const char *desc, const char *owner)
{
	UNUSED(len);
	UNUSED(pool);
	UNUSED(desc);
	UNUSED(owner);
	SEED_ABSTRACT_METHOD;

	return NULL;
}

INLINE void IMemoryManager::Free(void *ptr, IMemoryPool *pool)
{
	UNUSED(ptr);
	UNUSED(pool);
	SEED_ABSTRACT_METHOD;
}

INLINE void IMemoryManager::Info()
{
	SEED_ABSTRACT_METHOD;
}

INLINE BOOL IMemoryManager::IsRequired() const
{
	return TRUE;
}

INLINE const char *IMemoryManager::GetObjectName() const
{
	return "MemoryManager";
}

} // namespace
