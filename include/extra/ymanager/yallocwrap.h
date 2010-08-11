/*
Author.: Danny Angelo Carminati Grein (danny.cabelo@gmail.com)
Copyright (c) 2009 all rights reserved.
Description:

	This is a debugging memory allocator to analyze a program memory use,
	by no means this meant to be optimized and used in a production environment.
*/

#ifndef __YALLOCWRAP_H__
#define __YALLOCWRAP_H__

#include "ydefines.h"

class YMemoryManager;

YMemoryManager *ycreate(unsigned long size);
void ydestroy(YMemoryManager *mm);
void yfree(YMemoryManager *mm, void *ptr);
void *yalloc(YMemoryManager *mm, unsigned long size, const char *desc = NULL, const char *owner = NULL, int reserve = 0);

#endif // __YALLOCWRAP_H__
