/*! \file IphMemoryPool.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryPool Iphone Implementation
*/


#ifndef __IPH_MEMORY_POOL_H__
#define __IPH_MEMORY_POOL_H__

#ifdef _IPHONE_

#include <stdlib.h>
#include "interface/IMemoryPool.h"


namespace Seed { namespace iPhone {


class IphoneMemoryPool : public IMemoryPool
{
	friend class MemoryManager;

	public:
		IphoneMemoryPool();

		virtual void *Alloc(SIZE_T len, const char *desc, const char *owner);
		virtual void Free(void *ptr);
		virtual u32 GetFreeMemory() const;


	protected:
		virtual ~IphoneMemoryPool();


	private:
		IphoneMemoryPool(const IphoneMemoryPool &);
		IphoneMemoryPool &operator=(const IphoneMemoryPool &);
};


}} // namespace


#else // _IPHONE_

	#error "Include 'MemoryManager.h' instead 'platform/iphone/IphMemoryPool.h' directly."

#endif // _IPHONE_
#endif // __MEMORY_MANAGER__
