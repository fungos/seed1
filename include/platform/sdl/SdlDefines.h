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

/*! \file SdlDefines.h
	\author	Danny Angelo Carminati Grein
	\brief Global engine defines - all engine files must include this header
*/

#ifndef __SDL_DEFINES_H__
#define __SDL_DEFINES_H__

#if defined(_SDL_)

#define _OGL_		1
#define _PC_		1
#define _OAL_OGG_	1

#define _NO_W32_PSEUDO_MODIFIERS
#define OV_EXCLUDE_STATIC_CALLBACKS

#include <string.h>
#include <time.h>
#include <wchar.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#if defined(__APPLE_CC__)
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

#if defined(__MWERKS__)
#pragma warning off (10342)
#endif // __MWERKS__

#if defined(__MINGW32__)
	#if defined(SEED_BUILD_LGPL)
		#define SEED_CORE_API __declspec(dllexport)
	#elif defined(SEED_EXTRA_BUILD)
		#define SEED_PLATFORM_API __declspec(dllexport)
		#define SEED_EXTRA_API __declspec(dllexport)
		#define SEED_CORE_API __declspec(dllimport)
	#elif defined(SEED_USE_LGPL)
		#define SEED_CORE_API __declspec(dllimport)
		#define SEED_EXTRA_API __declspec(dllimport)
		#define SEED_PLATFORM_API _declspec(dllimport)
	#endif // __MINGW32__
#elif defined(_MSC_VER)
	#if defined(SEED_BUILD_LGPL)
		#define SEED_CORE_API _declspec(dllexport)
	#elif defined(SEED_EXTRA_BUILD)
		#define SEED_CORE_API _declspec(dllimport)
		#define SEED_EXTRA_API _declspec(dllexport)
		#define SEED_PLATFORM_API _declspec(dllexport)
	#elif defined(_MSC_VER) && defined(SEED_USE_LGPL)
		#define SEED_CORE_API __declspec(dllimport)
		#define SEED_EXTRA_API __declspec(dllimport)
		#define SEED_PLATFORM_API _declspec(dllimport)
	#endif // _MSC_VER
#endif

#define PLATFORM SDL

#if defined(_SDL_EMULATE_WII_)
	#define PLATFORM_MAX_SCREEN_WIDTH		640
	#define PLATFORM_MAX_SCREEN_HEIGHT		480
#elif _SDL_EMULATE_IPH_
	#define PLATFORM_MAX_SCREEN_WIDTH		480
	#define PLATFORM_MAX_SCREEN_HEIGHT		320
#else
	#define PLATFORM_MAX_SCREEN_WIDTH		1024
	#define PLATFORM_MAX_SCREEN_HEIGHT		768
#endif // _SDL_EMULATE_IPH_

#define PLATFORM_CODE					3
#define PLATFORM_MAX_INPUT				1
#define PLATFORM_SAVESYSTEM_SLOTS_MAX	10


// Types

#if defined(__CYGWIN__)
	#include <_G_config.h>
	typedef _G_int64_t			s64;
	typedef _G_uint64_t			u64;
	typedef _G_int32_t			s32;
	typedef _G_uint32_t			u32;
	typedef _G_int16_t			s16;
	typedef _G_uint16_t			u16;
#elif defined(__MINGW32__)
	typedef long long			s64;
	typedef unsigned long long	u64;
	typedef int					s32;
	typedef unsigned int		u32;
	typedef short				s16;
	typedef unsigned short		u16;
#elif defined(__MWERKS__)
	typedef long long			s64;
	typedef unsigned long long	u64;
	typedef int					s32;
	typedef unsigned int		u32;
	typedef short				s16;
	typedef unsigned short		u16;
#elif defined(__APPLE_CC__)
	typedef signed long long 		s64;
	typedef unsigned long long		u64;
	typedef signed int 			s32;
	typedef unsigned int 			u32;
	typedef signed short int 		s16;
	typedef unsigned short int 		u16;
#elif defined(__GNUC__)
	#include <inttypes.h>
	typedef uint64_t  			s64;
	typedef int64_t 			u64;
	typedef signed int 			s32;
	typedef unsigned int 			u32;
	typedef signed short int 		s16;
	typedef unsigned short int 		u16;
#else
	// MSVC and Borland
	typedef __int64				s64;
	typedef unsigned __int64	u64;
	typedef __int32				s32;
	typedef unsigned __int32	u32;
	typedef __int16				s16;
	typedef unsigned __int16	u16;
#endif
/*
typedef unsigned long int	 	u64;
typedef unsigned int 			u32;
typedef unsigned short int 		u16;
typedef signed long int 		s64;
typedef signed int 				s32;
typedef signed short int 		s16;
*/
typedef unsigned char 			u8;
typedef signed char 			s8;
typedef float 					f32;

typedef float 					fixed32;
typedef u32 					PIXEL;


#if defined(WIN32)
	#undef GetObject
	#undef BOOL
	#undef OUT
	#define ATTRIBUTE_ALIGN(x)
	#define PLATFORM_PATH_SEPARATOR			'\\'
	#if defined(_MSC_VER)
		#include <direct.h>
		#pragma warning(disable:4351)
		#define getcwd _getcwd
		#define snprintf _snprintf
		#define vsnprintf _vsnprintf
		//#define vsnwprintf _vsnwprintf
		#define chdir _chdir
		#define strdup _strdup
		#define vswprintf _vsnwprintf
	#endif // _MSC_VER
#else
	#define PLATFORM_PATH_SEPARATOR			'/'
#endif // WIN32


#define BOOL 	bool
#define FALSE	0
#define TRUE	1

#define LIB_SIZE_T				size_t
#define LIB_RAND				rand() //(u32)(srand((unsigned int)(time(NULL))))

// Memory and Strings
#define LIB_MEMSET					SDL_memset
#define LIB_MEMSET4(ptr, v, len)	do { int *p = static_cast<int *>(ptr); for (u32 x=0; x<len; x++) p[x]=v; } while (0)
#define LIB_MEMCOPY					SDL_memcpy
#define LIB_STRLEN					SDL_strlen
#define LIB_STRCMP					SDL_strcmp
#define LIB_STRLCPY(a, b, c)		SDL_strlcpy(a, b, c); a[c] = '\0';
#define LIB_STRLCAT					SDL_strlcat
#define LIB_STRCASECMP				SDL_strcasecmp
#define CHDIR						chdir
#define LIB_STRDUP					strdup

// Wide String funcs
#define LIB_WSTRLEN(ptr)			wcslen(ptr)
#define LIB_WVSNPRINTF				vswprintf

// Math
#define LIB_FAST_DIV(a, b)			(a/b)
#define LIB_FAST_MOD(a, b)			(a%b)
#define LIB_FAST_SQRT(a)			sqrt(a)
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

#if defined(WIN32)
#define HALT	do { __asm { int 3 }; exit(-1); } while (1);
#else
#define HALT	do { asm("int $3"); exit(-1); } while (1);
#endif // WIN32

// Debugging
#define LIB_ASSERT(x)				if (!(x)) { fprintf(stderr, "Failed assertion " #x); HALT; }
#define LIB_ASSERT_MSG(x, msg)		if (!(x)) { fprintf(stderr, msg #x); fflush(stderr); HALT; }
#define LIB_ASSERT_NULL(x)			if (!(x)) { fprintf(stderr, "Failed assertion " #x); HALT; }

#else // _SDL_

	#error "Include 'Defines.h' instead 'platform/sdl/SdlDefines.h' directly."

#endif // _SDL_
#endif // __SDL_DEFINES_H__
