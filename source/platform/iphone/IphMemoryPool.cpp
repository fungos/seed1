/*! \file IphMemoryPool.cpp
	\author	Danny Angelo Carminati Grein
	\brief MemoryPool Iphone Implementation
*/


#ifdef _IPHONE_


#include "MemoryManager.h"
#include <stdlib.h>


namespace Seed { namespace iPhone {



IphoneMemoryPool::IphoneMemoryPool()
{
}


IphoneMemoryPool::~IphoneMemoryPool()
{
}


void *IphoneMemoryPool::Alloc(SIZE_T len, const char *desc, const char *owner)
{
	return (void *)malloc(ROUND_UP(len, 32));
}


void IphoneMemoryPool::Free(void *ptr)
{
	free(ptr);
}


u32 IphoneMemoryPool::GetFreeMemory() const
{
	return 0;
}


}} // namespace


#endif // _IPHONE_
