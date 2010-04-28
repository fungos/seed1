/*
Author.: Danny Angelo Carminati Grein (danny.cabelo@gmail.com)
Copyright (c) 2009 all rights reserved.
Description:

	This is a debugging memory allocator to analyze a program memory use,
	by no means this meant to be optimized and used in a production environment.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <stdlib.h>
#include <stdio.h>

#if defined(YMEM_DEBUG)
	#if YMEM_DEBUG_LOG
		#define LOG0(arg);				printf(arg);
		#define LOG1(arg1, arg2);		printf(arg1, arg2);
		#define LOG2(arg1, arg2, arg3);	printf(arg1, arg2, arg3);
	#else
		#define LOG0(arg);
		#define LOG1(arg1, arg2);
		#define LOG2(arg1, arg2, arg3);
	#endif
	#define _YDEBUG(x)			x
#else
	#define LOG0(arg);
	#define LOG1(arg1, arg2);
	#define LOG2(arg1, arg2, arg3);
	#define _YDEBUG(x)
#endif // YMEM_DEBUG


//typedef u8 BOOL;

#define PTRSIZE		u64

#undef FALSE
#undef TRUE
#define FALSE		0
#define TRUE		1

#endif // __DEFINES_H__
