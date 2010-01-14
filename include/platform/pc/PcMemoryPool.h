/*! \file PcMemoryPool.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryPool PC Implementation
*/

#ifndef __PC_MEMORY_POOL_H__
#define __PC_MEMORY_POOL_H__

#include "Defines.h"

#if defined(_PC_)

#include <stdlib.h>
#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"
#include "extra/ymanager/ymemorymanager.h"


#ifdef DEBUG
#define YMANAGER_INIT	: pMgr(NULL)
#define YMANAGER_VAR	YMemoryManager *pMgr
#else
#define YMANAGER_INIT
#define YMANAGER_VAR
#endif // DEBUG


namespace Seed { namespace PC {


class PcMemoryPool : public IMemoryPool
{
	friend class MemoryManager;

	public:
		PcMemoryPool(u32 size);
		virtual void *Alloc(SIZE_T len, const char *desc, const char *owner);
		virtual void Free(void *ptr);
		virtual u32 GetFreeMemory() const;
		virtual void Print() const;

	protected:
		virtual void Initialize();
		virtual ~PcMemoryPool();

	private:
		SEED_DISABLE_COPY(PcMemoryPool);

	private:
		YMANAGER_VAR;
};


class DefaultMemoryPool : public PcMemoryPool
{
	friend class MemoryManager;

	public:
		DefaultMemoryPool(u32 size);

	protected:
		virtual ~DefaultMemoryPool();

	private:
		SEED_DISABLE_COPY(DefaultMemoryPool);
};


class LargeMemoryPool : public PcMemoryPool
{
	friend class MemoryManager;

	public:
		LargeMemoryPool(u32 size);

	protected:
		virtual ~LargeMemoryPool();

	private:
		SEED_DISABLE_COPY(LargeMemoryPool);
};


}} // namespace


#else // _PC_

	#error "Include 'MemoryManager.h' instead 'platform/pc/PcMemoryManager.h' directly."

#endif // _PC_
#endif // __PC_MEMORY_POOL_H__
