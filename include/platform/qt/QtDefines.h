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

/*! \file QtDefines.h
	\author	Danny Angelo Carminati Grein
	\brief Global engine defines - all engine files must include this header
*/

#ifndef __QT_DEFINES_H__
#define __QT_DEFINES_H__

#if defined(_QT_)

#include "platform/qt/QtPch.h"

#define _NO_W32_PSEUDO_MODIFIERS
#define _OGL_		1
#define _OAL_OGG_	1
#define _PC_		1

#if defined(_QT_EMULATE_WII_)
	#define PLATFORM_MAX_SCREEN_WIDTH		640
	#define PLATFORM_MAX_SCREEN_HEIGHT		480
#elif _QT_EMULATE_IPH_
	#define PLATFORM_MAX_SCREEN_WIDTH		480
	#define PLATFORM_MAX_SCREEN_HEIGHT		320
#else
	#define PLATFORM_MAX_SCREEN_WIDTH		1024
	#define PLATFORM_MAX_SCREEN_HEIGHT		768
#endif // _QT_EMULATE_IPH_

#define PLATFORM						QT
#define PLATFORM_CODE					3
#define PLATFORM_MAX_INPUT				1
#define PLATFORM_SAVESYSTEM_SLOTS_MAX	5

// Types
typedef unsigned long long int 	u64;
typedef unsigned int 			u32;
typedef unsigned short int 		u16;
typedef unsigned char 			u8;
typedef signed long long int	s64;
typedef signed int 				s32;
typedef signed short int 		s16;
typedef signed char 			s8;
typedef float 					f32;

typedef float 					fixed32;
typedef int 					PIXEL;
typedef int						BOOL;

#if defined(WIN32)
	#undef BOOL
	#define BOOL bool
	#undef OUT
	#define ATTRIBUTE_ALIGN(x)

	#if defined(_MSC_VER)
		#include <direct.h>
		#pragma warning(disable:4351)
		#define getcwd _getcwd
		#define snprintf _snprintf
		#define vsnprintf _vsnprintf
		#define chdir _chdir
	#endif // _MSC_VER
#endif // WIN32

#if !defined(FALSE)
#define FALSE	0
#endif // FALSE

#if !defined(TRUE)
#define TRUE	1
#endif // TRUE

#define LIB_SIZE_T				size_t
#define LIB_RAND				rand() //(u32)(srand((unsigned int)(time(NULL))))

// Memory amd Stings
#define LIB_MEMSET					memset
#define LIB_MEMSET4(ptr, v, len)	do { int *p = static_cast<int *>(ptr); for (u32 x=0; x<len; x++) p[x]=v; } while (0)
#define LIB_MEMCOPY					memcpy
#define LIB_STRLEN					strlen
#define LIB_STRCMP					strcmp
#define LIB_STRLCPY(a, b, c)		strncpy(a, b, c); a[c] = '\0';
#define LIB_STRLCAT					strncat
#define LIB_STRCASECMP				strcasecmp
#define CHDIR						chdir

// Wide String funcs
#define LIB_WSTRLEN(ptr)			wcslen(ptr)
#define LIB_WVSNPRINTF				vswprintf

// Math
#define LIB_FAST_DIV(a, b)			(a/b)
#define LIB_FAST_MOD(a, b)			(a%b)
#define LIB_FAST_SQRT(a)			sqrt(a)

// Display Object
#define LIB_PIXEL_R_MASK			0xff000000
#define LIB_PIXEL_R_SHIFT			24
#define LIB_PIXEL_G_MASK			0x00ff0000
#define LIB_PIXEL_G_SHIFT			16
#define LIB_PIXEL_B_MASK			0x0000ff00
#define LIB_PIXEL_B_SHIFT			8
#define LIB_PIXEL_A_MASK			0x000000ff
#define LIB_PIXEL_A_SHIFT			0
#define LIB_PIXEL_DEFAULT_KEYCOLOR 	0x00ff00ff
#define	LIB_PIXEL_COMPONENT(x)		(x&0xff)
#define	LIB_PIXEL_COLOR(r, g, b, a)	((LIB_PIXEL_COMPONENT(b)<<LIB_PIXEL_B_SHIFT) | (LIB_PIXEL_COMPONENT(g)<<LIB_PIXEL_G_SHIFT) | (LIB_PIXEL_COMPONENT(r)<<LIB_PIXEL_R_SHIFT) | (LIB_PIXEL_COMPONENT(a)<<LIB_PIXEL_A_SHIFT))
#define LIB_PIXEL_ALPHA_MAX_VALUE	255

union uPixel
{
	PIXEL pixel;
	struct _sPixel
	{
		u8 c[4];
	} pComponent;
	struct _cPixel
	{
		u8 a;
		u8 b;
		u8 g;
		u8 r;
	} component;
};

#define HALT	do { exit(-1); } while (1);

// Debugging
#define LIB_ASSERT(x)				if (!(x)) { fprintf(stderr, "Failed assertion " #x); HALT; }
#define LIB_ASSERT_MSG(x, msg)		if (!(x)) { fprintf(stderr, msg #x); HALT; }
#define LIB_ASSERT_NULL(x)			if (!((void *)x)) { fprintf(stderr, "Failed assertion " #x); HALT; }

#else // _QT_
	#error "Include 'Defines.h' instead 'platform/qt/QtDefines.h' directly."
#endif // _QT_
#endif // __QT_DEFINES_H__
