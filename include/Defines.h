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

/*! \file Defines.h
	\author	Danny Angelo Carminati Grein
	\brief Defines
*/

#ifndef __SEED_DEFINES_H__
#define __SEED_DEFINES_H__

#include "Config.h"

/*
Compiler specific pragmas here

About warning 4251 - DLL export for templatized classes (std and others)
http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
*/
#if defined(_MSC_VER)
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4530)
#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4251) // stl + dll
#endif

#if defined(_WII_)
	#include "platform/wii/WiiDefines.h"
#else
	#if defined(_SDL_)
		#include "platform/sdl/SdlDefines.h"
	#else
		#if defined(_IPHONE_)
			#include "platform/iphone/IphDefines.h"
		#else
			#if defined(_QT_)
				#include "platform/qt/QtDefines.h"
			#endif // _QT_
		#endif // IPHONE
	#endif // SDL
#endif // WII

#ifndef SEED_CORE_API
#define SEED_CORE_API
#endif

#ifndef SEED_PLATFORM_API
#define SEED_PLATFORM_API
#endif

#ifndef SEED_EXTRA_API
#define SEED_EXTRA_API
#endif

#define STRING_LENGTH					LIB_STRLEN
#define SIZE_T							LIB_SIZE_T

#define UNUSED(var)						(void)var;

#define RAND							LIB_RAND

#undef MEMSET4
// Memory and Strings
#define MEMSET							LIB_MEMSET
#define MEMSET4							LIB_MEMSET4
#define MEMCOPY							LIB_MEMCOPY
#define STRLEN							LIB_STRLEN
#define STRCMP							LIB_STRCMP
#define STRLCPY							LIB_STRLCPY
#define STRLCAT							LIB_STRLCAT
#define STRCASECMP						LIB_STRCASECMP
#define STRDUP							LIB_STRDUP

// Wide String funcs
#define WSTRLEN							LIB_WSTRLEN
#define WVSNPRINTF						LIB_WVSNPRINTF

// Math
#define FAST_DIV						LIB_FAST_DIV
#define FAST_MOD						LIB_FAST_MOD
#define FAST_SQRT						LIB_FAST_SQRT

// Display Object
#define PIXEL_A_MASK					LIB_PIXEL_A_MASK
#define PIXEL_A_SHIFT					LIB_PIXEL_A_SHIFT
#define PIXEL_R_MASK					LIB_PIXEL_R_MASK
#define PIXEL_R_SHIFT					LIB_PIXEL_R_SHIFT
#define PIXEL_G_MASK					LIB_PIXEL_G_MASK
#define PIXEL_G_SHIFT					LIB_PIXEL_G_SHIFT
#define PIXEL_B_MASK					LIB_PIXEL_B_MASK
#define PIXEL_B_SHIFT					LIB_PIXEL_B_SHIFT
#define PIXEL_DEFAULT_KEYCOLOR			LIB_PIXEL_DEFAULT_KEYCOLOR
#define	PIXEL_COMPONENT					LIB_PIXEL_COMPONENT
#define	PIXEL_COLOR						(PIXEL)LIB_PIXEL_COLOR
#define PIXEL_ALPHA_MAX_VALUE			LIB_PIXEL_ALPHA_MAX_VALUE

#define PIXEL_GET_A(px)			(static_cast<u8>(((px & PIXEL_A_MASK) >> PIXEL_A_SHIFT)))
#define PIXEL_GET_R(px)			(static_cast<u8>(((px & PIXEL_R_MASK) >> PIXEL_R_SHIFT)))
#define PIXEL_GET_G(px)			(static_cast<u8>(((px & PIXEL_G_MASK) >> PIXEL_G_SHIFT)))
#define PIXEL_GET_B(px)			(static_cast<u8>(((px & PIXEL_B_MASK) >> PIXEL_B_SHIFT)))
#define PIXEL_SET_A(px, a)		(((px) & (~LIB_PIXEL_A_MASK)) + (static_cast<u32>(a)<<PIXEL_A_SHIFT))
#define PIXEL_SET_R(px, r)		(((px) & (~LIB_PIXEL_R_MASK)) + (static_cast<u32>(r)<<PIXEL_R_SHIFT))
#define PIXEL_SET_G(px, g)		(((px) & (~LIB_PIXEL_G_MASK)) + (static_cast<u32>(g)<<PIXEL_G_SHIFT))
#define PIXEL_SET_B(px, b)		(((px) & (~LIB_PIXEL_B_MASK)) + (static_cast<u32>(b)<<PIXEL_B_SHIFT))

union uPixel
{
	PIXEL pixel;
	struct vec
	{
		u8 c[4];
	} pComponent;
	struct _argb
	{
		u8 a;
		u8 r;
		u8 g;
		u8 b;
	} argb;
	struct _rgba
	{
		u8 r;
		u8 g;
		u8 b;
		u8 a;
	} rgba;

	uPixel(u8 R, u8 G, u8 B, u8 A)
		: pixel(PIXEL_COLOR(R,G,B,A))
	{
	}

	uPixel()
		: pixel(0)
	{
	}
};

// Debugging
#if defined(DEBUG)
	#ifndef ASSERT
	#define ASSERT							LIB_ASSERT
	#endif // ASSERT

	#ifndef ASSERT_MSG
	#define ASSERT_MSG						LIB_ASSERT_MSG
	#endif // ASSERT_MSG

	#ifndef ASSERT_NULL
	#define ASSERT_NULL						LIB_ASSERT_NULL
	#endif // ASSERT_NULL

	#ifndef SECURITY_CHECK
	#define SECURITY_CHECK					LIB_ASSERT_MSG
	#endif // SECURITY_CHECK

	#if defined(__GNUC__)
		#define __FUNC__					__PRETTY_FUNCTION__
	#else
		#define __FUNC__					__FUNCSIG__
	#endif

	#define SEED_ABSTRACT_METHOD		Dbg(SEED_TAG "WARNING: Calling an 'abstract' method: [%s] (%s:%d).", __FUNC__, __FILE__, __LINE__);

	#define SEED_DISABLE_INSTANCING		public: \
											friend class LeakReport; \
										protected: \
											void *operator new(size_t len); \
											void operator delete(void *ptr); \
											void *operator new[](size_t) throw() { return NULL; }; \
											void operator delete[](void *) {};

#else
	#define SEED_ABSTRACT_METHOD

	#define SEED_DISABLE_INSTANCING		protected: \
											void *operator new(size_t len); \
											void operator delete(void *ptr); \
											void *operator new[](size_t) throw() { return NULL; }; \
											void operator delete[](void *) {};

	#if defined(__GNUC__) || defined(_WII_)
		#ifndef ASSERT
		#define ASSERT(...)
		#endif // ASSERT

		#ifndef ASSERT_MSG
		#define ASSERT_MSG(...)
		#endif // ASSERT_MSG

		#ifndef ASSERT_NULL
		#define ASSERT_NULL(...)
		#endif // ASSERT_NULL
	#else
		#ifndef ASSERT
		#define ASSERT
		#endif // ASSERT

		#ifndef ASSERT_MSG
		#define ASSERT_MSG
		#endif // ASSERT_MSG

		#ifndef ASSERT_NULL
		#define ASSERT_NULL
		#endif // ASSERT_NULL
	#endif // __GNUC__

	#ifndef SECURITY_CHECK
	#define SECURITY_CHECK(func, msg)		func
	#endif // SECURITY_CHECK

#endif // DEBUG

#define SEED_INVALID_ID					0xFFFFFFFF

#define CLAMP(val,min,max) 				((val) = (((val) < (min)) ? (min) : ((val) > (max)) ? (max) : (val)))
#define ROUND_UP(value, alignment)		(((u32)(value) + (alignment-1)) & ~(alignment-1))
#define ROUND_DOWN(value, alignment)	((u32)(value) & ~(alignment-1))
#define PTR_OFF(ptr) 			((size_t)(ptr))
#define ALIGN_OFFSET(ptr, align)	(PTR_OFF(ptr) & ((align) - 1))
#define ALIGN_FLOOR(ptr, align)		((u8 *)(ptr) - ( PTR_OFF(ptr) & ((align) - 1)))
#define ALIGN_CEIL(ptr, align) 		((u8 *)(ptr) + (-PTR_OFF(ptr) & ((align) - 1)))

#define DISPLAY_MEM()					pMemoryManager->Info()


#define SEED_DISABLE_COPY(Class)		Class(const Class &); \
										Class &operator=(const Class &)


#define SEED_DISABLE_INSTANCING_IMPL(Class)	\
										INLINE void *Class::operator new(size_t len) \
										{ \
											return pMemoryManager->Alloc(len); \
										} \
										 \
										INLINE void Class::operator delete(void *ptr) \
										{ \
											pMemoryManager->Free(ptr); \
										}

#define SEED_FORWARD_DECLARATION(Class) namespace Seed { class Class; }


#define SEED_COMPILE_TIME_ASSERT(name, x)	typedef int __seed_compile_assert_ ## name[(x) * 2 - 1]

SEED_COMPILE_TIME_ASSERT(u8,  sizeof(u8) == 1);
SEED_COMPILE_TIME_ASSERT(s8,  sizeof(s8) == 1);
SEED_COMPILE_TIME_ASSERT(u16, sizeof(u16) == 2);
SEED_COMPILE_TIME_ASSERT(s16, sizeof(s16) == 2);
SEED_COMPILE_TIME_ASSERT(u32, sizeof(u32) == 4);
SEED_COMPILE_TIME_ASSERT(s32, sizeof(s32) == 4);
SEED_COMPILE_TIME_ASSERT(u64, sizeof(u64) == 8);
SEED_COMPILE_TIME_ASSERT(s64, sizeof(s64) == 8);

typedef enum { SEED_ENUM_ASSERT_VALUE } SEED_ENUM_ASSERT;
SEED_COMPILE_TIME_ASSERT(enum, sizeof(SEED_ENUM_ASSERT) == sizeof(u32));

/*
typedef u16* WideString;
typedef u16  WideChar;
*/

#define WideString 	u16*
#define WideChar	u16

#endif // __SEED_DEFINES_H__
