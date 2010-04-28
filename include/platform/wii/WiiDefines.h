/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file WiiDefines.h
	\author	Danny Angelo Carminati Grein
	\brief Wii specific defines
*/

#ifndef __WII_DEFINES_H__
#define __WII_DEFINES_H__

#if defined(_WII_)

#include <string.h>
#include <wchar.h>
#include "WiiWrapper.h"

#define PLATFORM						WII

#define PLATFORM_MAX_SCREEN_WIDTH		640
#define PLATFORM_MAX_SCREEN_HEIGHT		480
#define PLATFORM_CODE					1
#define PLATFORM_MAX_INPUT				WII_MAX_CONTROLLERS
#define PLATFORM_THREAD_STACK_SIZE		4096
#define PLATFORM_PATH_SEPARATOR			'/'
#define PLATFORM_SAVESYSTEM_SLOTS_MAX	4
// Types
/*
typedef unsigned long long int 	u64;
typedef unsigned int 			u32;
typedef unsigned short int 		u16;
typedef unsigned char 			u8;
typedef signed long long int 	s64;
typedef signed int 				s32;
typedef signed short int 		s16;
typedef signed char 			s8;
typedef float 					f32;
*/
typedef float 					fixed32;

//typedef unsigned char 		BOOL;
typedef int 					PIXEL;

//#define FALSE	0
//#define	TRUE	1

#define LIB_SIZE_T				size_t
#define LIB_RAND				WiiGetTick() //(u32)(srand((unsigned int)(time(NULL))))

// Memory and Strings
#define LIB_MEMSET(ptr, v, len)		memset(ptr, v, len)
#define LIB_MEMSET4(ptr, v, len)	do { int *p = static_cast<int *>(ptr); for (int x=0; x<len; x++) p[x]=v; } while (0)
#define LIB_MEMCOPY(dst, src, len)  memcpy(dst, src, len)
#define LIB_STRLEN(ptr)				strlen(ptr)
#define LIB_STRCMP					strcmp
#define LIB_STRLCPY(a, b, c)		strncpy(a, b, c); a[c] = '\0'; // Wii does not implement strlcpy
#define LIB_STRLCAT(a, b, c)		strcat(a, b) // Wii does not implement strlcat
#define LIB_STRCASECMP				strcasecmp
#define LIB_STRDUP					strdup

// Wide String funcs
#define LIB_WSTRLEN(ptr)			wcslen(ptr)
#define LIB_WVSNPRINTF				vswprintf

// Math
#define LIB_FAST_DIV(a, b)			((a)/b)
#define LIB_FAST_MOD(a, b)			((a)%b)
#define LIB_FAST_SQRT(a)			sqrt(a)
#define LIB_FAST_COS(a)				cos(a)
#define LIB_FAST_SIN(a)				sin(a)
#define LIB_FAST_ACOS(a)			acos(a)
#define LIB_FAST_ASIN(a)			asin(a)
#define LIB_ABS(a)			    	fabs(a)

// Display Object

// NDS
//#define DISPLAY_OBJECT_DMA_CHANNEL 3
//#define DEFAULT_KEYCOLOR 0xFC1F //0111110000011111
//#define	BIT5(x)		((x)>>3)
//#define	RGBA5551(r, g, b, a)		((BIT5(r)<<0) | (BIT5(g)<<5) | (BIT5(b)<<10)| (((a)&1)<<15))

#define LIB_PIXEL_A_MASK			0x000000ff
#define LIB_PIXEL_A_SHIFT			0
#define LIB_PIXEL_R_MASK			0xff000000
#define LIB_PIXEL_R_SHIFT			24
#define LIB_PIXEL_G_MASK			0x00ff0000
#define LIB_PIXEL_G_SHIFT			16
#define LIB_PIXEL_B_MASK			0x0000ff00
#define LIB_PIXEL_B_SHIFT			8
#define LIB_PIXEL_DEFAULT_KEYCOLOR 	0xff00ff00
/*#define LIB_PIXEL_A_MASK			0xff000000
#define LIB_PIXEL_A_SHIFT			24
#define LIB_PIXEL_R_MASK			0x00ff0000
#define LIB_PIXEL_R_SHIFT			16
#define LIB_PIXEL_G_MASK			0x0000ff00
#define LIB_PIXEL_G_SHIFT			8
#define LIB_PIXEL_B_MASK			0x000000ff
#define LIB_PIXEL_B_SHIFT			0
#define LIB_PIXEL_DEFAULT_KEYCOLOR 	0xffff00ff*/
#define	LIB_PIXEL_COMPONENT(x)		(x&0xff)
#define	LIB_PIXEL_COLOR(r, g, b, a)	(PIXEL)((LIB_PIXEL_COMPONENT(b)<<LIB_PIXEL_B_SHIFT) | (LIB_PIXEL_COMPONENT(g)<<LIB_PIXEL_G_SHIFT) | (LIB_PIXEL_COMPONENT(r)<<LIB_PIXEL_R_SHIFT) | (LIB_PIXEL_COMPONENT(a)<<LIB_PIXEL_A_SHIFT))
#define LIB_PIXEL_ALPHA_MAX_VALUE	255

// Debugging
#define LIB_ASSERT(x)				if (!(x)) { WiiPanic(__FILE__, __LINE__, "Failed assertion " #x); }
#define LIB_ASSERT_MSG(x, msg)		if (!(x)) { WiiPanic(__FILE__, __LINE__, msg); }
#define LIB_ASSERT_NULL(x)			if ((x) == NULL) { WiiPanic(__FILE__, __LINE__, "Failed null assertion of " #x); }

#define HALT						WiiHalt("Halt!");

//#define WII_CLAMP(x,l,h) 			((x > h) ? h : ((x < l) ? l : x))
//#define WII_TENT(a, b, c) 		(0.25 * (a) + 0.5 * (b) + 0.25 * (c))

#define ENABLE_SCREENSHOT_TOOL

#else // _WII_
	#error "Include 'Defines.h' instead 'platform/wii/WiiDefines.h' directly."
#endif // _WII_
#endif // __WII_DEFINES_H__
