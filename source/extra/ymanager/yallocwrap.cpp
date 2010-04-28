/*
Author.: Danny Angelo Carminati Grein (danny.cabelo@gmail.com)
Copyright (c) 2009 all rights reserved.
Description:

	This is a debugging memory allocator to analyze a program memory use,
	by no means this meant to be optimized and used in a production environment.
*/

#include "extra/ymanager/ydefines.h"
#include "extra/ymanager/yallocwrap.h"
#include "extra/ymanager/ymemorymanager.h"
#include <new>

YMemoryManager *ycreate(unsigned long size)
{
	void *p = (YMemoryManager *)(malloc(sizeof(YMemoryManager)));
	YMemoryManager *mm = new(p) YMemoryManager(size);

	return mm;
}

void ydestroy(YMemoryManager *mm)
{
	if (!mm)
		return;

	mm->~YMemoryManager();
	free(mm);
	mm = NULL;
}


void *yalloc(YMemoryManager *mm, unsigned long size, const char *desc, const char *owner)
{
	if (!mm)
		return NULL;

#if defined(YMEM_DEBUG)
	void *ptr = mm->Alloc(size, desc, owner);
	//mm->PrintSnapshot();
#else
	UNUSED(desc);
	UNUSED(owner);
	void *ptr = mm->Alloc(size);
#endif // YMEM_DEBUG

	return ptr;
}

void yfree(YMemoryManager *mm, void *ptr)
{
	if (!mm)
		return;

	mm->Free(ptr);

#if defined(YMEM_DEBUG)
	//mm->PrintSnapshot();
#endif // YMEM_DEBUG
}
