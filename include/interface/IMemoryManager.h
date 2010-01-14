/*! \file IMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Manager class interface
*/

#ifndef __IMEMORYMANAGER_H__
#define __IMEMORYMANAGER_H__

#include "IModule.h"


namespace Seed {

class IMemoryPool;


/// MemoryManager Interface
/**
*/
class IMemoryManager : public IModule
{
	public:
		IMemoryManager();
		virtual ~IMemoryManager();

		virtual void *Alloc(SIZE_T len, IMemoryPool *pool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool);
		virtual void Info();

		virtual u32 GetFreeMemory() const;

	private:
		SEED_DISABLE_COPY(IMemoryManager);
};


} // namespace


#endif // __IMEMORYMANAGER_H__
