/*! \file IMemoryPool.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Pool class interface
*/


#ifndef __IMEMORYPOOL_H__
#define __IMEMORYPOOL_H__


#include "Defines.h"


namespace Seed {


/// MemoryPool Interface
/**
Systems that has multiple ways or kinds of memory must implement its own MemoryPool. At least
one MemoryPool is needed and it must be called DefaultMemoryPool.
*/
class IMemoryPool
{
	public:
		IMemoryPool();

		virtual void *Alloc(SIZE_T len, const char *desc, const char *owner) = 0;
		virtual void Free(void *ptr) = 0;
		virtual u32 GetFreeMemory() const;
		virtual void Print() const;

		u32 iAllocations;
		u32 iTotalAllocations;

	protected:
		virtual ~IMemoryPool();

	private:
		SEED_DISABLE_COPY(IMemoryPool);
};


} // namespace


#endif // __IMEMORYPOOL_H__
