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

/*! \file IphDefines.h
	\author	Danny Angelo Carminati Grein
	\brief Global engine defines - all engine files must include this header
*/


#ifndef __IPH_DEFINES_H__
#define __IPH_DEFINES_H__

#if defined(_IPHONE_)

#include <string.h>

#define PLATFORM iPhone

// Workaround EAGL.h including Debugging.h and giving error on Release build.
#if defined(RELEASE)
	#define __DEBUGGING__
#endif // RELEASE

#define PLATFORM_MAX_SCREEN_WIDTH		480
#define PLATFORM_MAX_SCREEN_HEIGHT		320
#define PLATFORM_CODE					2
#define PLATFORM_MAX_INPUT				2
#define PLATFORM_PATH_SEPARATOR			'/'
#define PLATFORM_SAVESYSTEM_SLOTS_MAX	4

// Types
typedef unsigned long long int 	u64;
typedef unsigned int 			u32;
typedef unsigned short int 		u16;
typedef unsigned char 			u8;
typedef signed long long		s64;
typedef signed int 				s32;
typedef signed short int 		s16;
typedef signed char 			s8;
typedef float 					f32;

typedef float 					fixed32;

typedef signed char 			BOOL;
typedef int 					PIXEL;

#define FALSE	0
#define TRUE	1

#define LIB_SIZE_T				size_t

// Memory amd Stings
#define LIB_MEMSET(ptr, v, len)		memset(ptr, v, len)
#define LIB_MEMSET4(ptr, v, len)	do { int *p = static_cast<int *>(ptr); for (u32 x=0; x<len; x++) p[x]=v; } while (0)
#define LIB_MEMCOPY(dst, src, len)  memcpy(dst, src, len)
#define LIB_STRLEN(ptr)				strlen(ptr)
#define LIB_STRCMP					strcmp
#define LIB_STRLCPY					strlcpy
#define LIB_STRLCAT					strlcat
#define LIB_STRCASECMP				strcasecmp
#define LIB_STRDUP					strdup

// Math
#define LIB_FAST_DIV(a, b)			(a/b)
#define LIB_FAST_MOD(a, b)			(a%b)
#define LIB_FAST_SQRT(a)			(float)sqrt(a) // FIXME: Double...
#define LIB_FAST_COS(a)				cos(a)
#define LIB_FAST_SIN(a)				sin(a)
#define LIB_FAST_ACOS(a)			acos(a)
#define LIB_FAST_ASIN(a)			asin(a)
#define LIB_ABS(a)			    	fabs(a)

// Display Object
#define LIB_PIXEL_R_MASK			0xff000000
#define LIB_PIXEL_R_SHIFT			24
#define LIB_PIXEL_G_MASK			0x00ff0000
#define LIB_PIXEL_G_SHIFT			16
#define LIB_PIXEL_B_MASK			0x0000ff00
#define LIB_PIXEL_B_SHIFT			8
#define LIB_PIXEL_A_MASK			0x000000ff
#define LIB_PIXEL_A_SHIFT			0
#define LIB_PIXEL_DEFAULT_KEYCOLOR 	0xff00ffff
#define	LIB_PIXEL_COMPONENT(x)		(x&0xff)
#define	LIB_PIXEL_COLOR(r, g, b, a)	((LIB_PIXEL_COMPONENT(b)<<LIB_PIXEL_B_SHIFT) | (LIB_PIXEL_COMPONENT(g)<<LIB_PIXEL_G_SHIFT) | (LIB_PIXEL_COMPONENT(r)<<LIB_PIXEL_R_SHIFT) | (LIB_PIXEL_COMPONENT(a)<<LIB_PIXEL_A_SHIFT))
#define LIB_PIXEL_ALPHA_MAX_VALUE	255

#define _Halt(x)	Log("%s", x); while (1);
#define HALT		do { Log("HALT!"); exit(-1); } while (1);

// Debugging
#define LIB_ASSERT(x)				if (!(x)) { Log("%s:%d", __FILE__, __LINE__); _Halt("Failed assertion " #x); }
#define LIB_ASSERT_MSG(x, msg)		if (!(x)) { Log("%s:%d", __FILE__, __LINE__); _Halt(msg); }
#define LIB_ASSERT_NULL(x)			if ((x) == NULL) { Log("%s:%d", __FILE__, __LINE__); _Halt("Failed NULL assertion on " #x); }

//#define ENABLE_NATIVE_PVRTC_FORMAT		// MUST HAVE SEED_ENABLE_PRELOAD_TEXTURE enabled!

#else // _IPHONE_
	#error "Include 'Defines.h' instead 'platform/iphone/IphDefines.h' directly."
#endif // _IPHONE_
#endif // __IPH_DEFINES_H__
