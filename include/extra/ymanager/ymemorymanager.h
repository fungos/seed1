/*
Author.: Danny Angelo Carminati Grein (danny.cabelo@gmail.com)
Copyright (c) 2009 all rights reserved.
Description:

	This is a debugging memory allocator to analyze a program memory use,
	by no means this meant to be optimized and used in a production environment.
*/

#ifndef __YMANAGER_H__
#define __YMANAGER_H__

#include "Defines.h"
#include "ydefines.h"

class SEED_CORE_API YMemoryManager
{
	friend YMemoryManager *ycreate(unsigned long size);
	friend void ydestroy(YMemoryManager *mm);
	friend void *yalloc(YMemoryManager *mm, unsigned long size, const char *desc, const char *owner);
	friend void yfree(YMemoryManager *mm, void *ptr);

	public:
		u32 GetFreeMemory();

		_YDEBUG
		(
			void PrintSnapshot();
			u32 GetOverrunCount();
			u32 GetAllocationCount();
			u32 GetFreeCount();
			u32 GetAllocationTotalBytes();
		)

	private:
		YMemoryManager(u32 nbytes);
		~YMemoryManager();

		void *Alloc(u32 nbytes, const char *desc = NULL, const char *owner = NULL);
		void Free(void *addr);

	private:
		SEED_DISABLE_COPY(YMemoryManager);

		void Split(u32 addr, u32 nbytes, const char *desc, const char *owner);
		void Merge(u32 prev, u32 current, u32 next);

	private:
		u8		*pPool;
		u32		iSize;
		u32		iFreeMemory;

		_YDEBUG
		(
			u32		iFreeCount;
			u32		iAllocationCount;
			u32		iOverrunCount;
			u32		iAllocationTotalBytes;
		)
};

#endif // __YMANAGER_H__
