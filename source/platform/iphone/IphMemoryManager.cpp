/*! \file IphMemoryManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager Iphone Implementation
*/

#ifdef _IPHONE_

#include "Defines.h"
#include "MemoryManager.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/sysctl.h>

#define TAG "[MemoryManager] "


namespace Seed { namespace iPhone {


MemoryManager MemoryManager::instance;
IphoneMemoryPool MemoryManager::defaultPool;


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


INLINE u32 MemoryManager::GetFreeMemory() const
{
	return 0;
}


INLINE void *MemoryManager::Alloc(SIZE_T len, IMemoryPool *pool, const char *desc, const char *owner)
{
	//IphoneMemoryPool *p = static_cast<IphoneMemoryPool *>(pool);
	return (void *)malloc(ROUND_UP(len, 32));
}


INLINE void MemoryManager::Free(void *ptr, IMemoryPool *pool)
{
	free(ptr);
}


INLINE void MemoryManager::Info()
{
}


}} // namespace


#endif // _IPHONE_
