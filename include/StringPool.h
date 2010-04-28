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

/*! \file StringPool.h
	\author	Danny Angelo Carminati Grein
	\brief Pool of allocated string buffers with a variety of sizes.
*/

#ifndef __STRING_POOL_H__
#define __STRING_POOL_H__

#include "Defines.h"

#if SEED_USE_STRING_POOL == 1

#include "Log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TString.h"

#if defined(DEBUG)
#define DBLOCK(x)	x
#else
#define DBLOCK(x)
#endif

/*
Define buffers amount
*/
#define STRING_POOL_32		SEED_STRING_POOL_32
#define STRING_POOL_64		SEED_STRING_POOL_64
#define STRING_POOL_128		SEED_STRING_POOL_128
#define STRING_POOL_256		SEED_STRING_POOL_256
#define STRING_POOL_512		SEED_STRING_POOL_512
#define STRING_POOL_1024	SEED_STRING_POOL_1024
#define STRING_POOL_2048	SEED_STRING_POOL_2048
#define STRING_POOL_4096	SEED_STRING_POOL_4096

#define STRING_POOL_FRONTGUARD	(u32)0xdeadbeef
#define STRING_POOL_BACKGUARD	(u32)0xbeefdead
#define STRING_POOL_BUSY		(u32)0x12132000
#define STRING_POOL_UNUSED		(u32)0x00000000

#define FIND_POOL(size)		for (int j = 0; j < STRING_POOL_##size; j++) \
							{ \
								if (_p##size.mBuff[j].iFlags == STRING_POOL_UNUSED) \
								{ \
									p = _p##size.mBuff[j].pData; \
									_p##size.mBuff[j].iFlags = STRING_POOL_BUSY|size; \
									iCurrentCount##size++; \
									iTotalCount##size++; \
									break; \
								} \
							}

#define CHECK_POOL(size, leak)	for (int i = 0; i < STRING_POOL_##size  ; i++) \
							{ \
								u32 front = _p##size.mBuff[i].iFrontGuard; \
								u32 back = _p##size.mBuff[i].iBackGuard; \
								u32 flags = _p##size.mBuff[i].iFlags; \
								T *data = _p##size.mBuff[i].pData; \
								int len = flags & 0x00001FFF; \
								if (((flags & STRING_POOL_BUSY) != STRING_POOL_BUSY && flags != 0) || front != STRING_POOL_FRONTGUARD) \
								{ \
									char *str = reinterpret_cast<char *>(&_p##size.mBuff[i].iFlags); \
									Log("[StringPool] SMASHED: %d [%c%c%c%c]", size, str[0], str[1], str[2], str[3]); \
								} \
								else if ((len && Length(data) > size) || back != STRING_POOL_BACKGUARD) \
								{ \
									Log("[StringPool] OVERRUN: %d [%d]", size, len); \
								} \
								else if (leak && (flags & STRING_POOL_BUSY) == STRING_POOL_BUSY) \
								{ \
									Log("[StringPool] LEAK: %d [%d]", size, flags&0x00001FFF); \
								} \
							}


#define PRINT_POOL(size)	Log("[StringPool] - %d Pool Allocated: %d", size, iTotalCount##size); \
							for (int i = 0; i < STRING_POOL_##size  ; i++) \
							{ \
								u32 flags = _p##size.mBuff[i].iFlags; \
								T *data = _p##size.mBuff[i].pData; \
								int len = flags & 0x00001FFF; \
								if (flags) \
								{ \
									fprintf(stdout, "[StringPool] POOL: %4d [%4d] [", size, len); \
									for (int j = 0; j < size; j++) fprintf(stdout, "%c", data[j]?data[j]:'.'); \
									fprintf(stdout, "] %d\n", (int)String::Length(data)); \
								} \
							} \


#define b2(x)   (   (x) | (   (x) >>  1))
#define b4(x)   ( b2(x) | ( b2(x) >>  2))
#define b8(x)  	( b4(x) | ( b4(x) >>  4))
#define b16(x)  ( b8(x) | ( b8(x) >>  8))
#define b32(x)	(b16(x) | (b16(x) >> 16))
#define np2(x) 	(b32(x-1) + 1)

namespace Seed {

template <int SIZE, int COUNT, typename T> struct SEED_CORE_API StringPool
{
	StringPool() {}
	virtual ~StringPool() {}

#if defined(DEBUG)
	struct SEED_CORE_API StringEntry
	{
		u32 iFrontGuard;
		u32 iFlags;
		T pData[SIZE];
		u32 iBackGuard;

		StringEntry()
			: iFrontGuard(STRING_POOL_FRONTGUARD)
			, iFlags(STRING_POOL_UNUSED)
			, pData()
			, iBackGuard(STRING_POOL_BACKGUARD)
		{}// { memset(pData, '\0', SIZE); }
	} mBuff[COUNT];
#else
	struct SEED_CORE_API StringEntry
	{
		int iFlags;
		T pData[SIZE];
	} mBuff[COUNT];
#endif
};

#define DEFINE_POOL(size)	StringPool<size, STRING_POOL_##size, T> _p##size; u32 iCurrentCount##size; u32 iTotalCount##size;

template <typename T> class SEED_CORE_API StringPoolManager
{
	public:
		StringPoolManager()
			: _p32()
			, iCurrentCount32(0)
			, iTotalCount32(0)
			, _p64()
			, iCurrentCount64(0)
			, iTotalCount64(0)
			, _p128()
			, iCurrentCount128(0)
			, iTotalCount128(0)
			, _p256()
			, iCurrentCount256(0)
			, iTotalCount256(0)
			, _p512()
			, iCurrentCount512(0)
			, iTotalCount512(0)
			, _p1024()
			, iCurrentCount1024(0)
			, iTotalCount1024(0)
			, _p2048()
			, iCurrentCount2048(0)
			, iTotalCount2048(0)
			, _p4096()
			, iCurrentCount4096(0)
			, iTotalCount4096(0)
			, iTotalCount(0)
			, iCurrentCount(0)
		{
		}

		~StringPoolManager<T>()
		{
		}

		u32 Length(T *ptr)
		{
			u32 i = 0;
			while (ptr[i] != NULL) i++;

			return i;
		}

		void Free(T *p)
		{
			long int adrp = (long int)p;
			if (adrp > (int)sizeof(int))
			{
				int offset = sizeof(u32);
				int tail = 0;

				DBLOCK
				(
					int *front = reinterpret_cast<int *>((char*)p - sizeof(u32) - sizeof(u32));
					if (*front != (int)STRING_POOL_FRONTGUARD)
					{
						Log("[StringPool] WARNING: String at 0x%lx has been smashed.", (long int)p);
					}

					offset += sizeof(u32);
					tail = sizeof(u32);
				)

				T *bp = (T*)((char*)p - offset);
				int size = 0;

				DBLOCK(
					int *flags = reinterpret_cast<int *>((char*)p - sizeof(u32));
					if (*flags)
					{
						if (adrp < (long int)&_p64)
						{
							size = 32;
						}
						else if (adrp < (long int)&_p128)
						{
							size = 64;
						}
						else if (adrp < (long int)&_p256)
						{
							size = 128;
						}
						else if (adrp < (long int)&_p512)
						{
							size = 256;
						}
						else if (adrp < (long int)&_p1024)
						{
							size = 512;
						}
						else if (adrp < (long int)&_p2048)
						{
							size = 1024;
						}
						else if (adrp < (long int)&_p4096)
						{
							size = 2048;
						}
						else
						{
							size = 4096;
						}
					}
				)

				memset(bp, '\0', size * sizeof(T) + offset + tail);
				//*flags = STRING_POOL_UNUSED;

				DBLOCK
				(
					int *fg = reinterpret_cast<int *>(bp);
					*fg = STRING_POOL_FRONTGUARD;

					int *bg = reinterpret_cast<int *>(p + size);
					*bg = STRING_POOL_BACKGUARD;
				)
			}

			ASSERT(iCurrentCount > 0);
			iCurrentCount--;
		}

		T *Alloc(int size)
		{
			T *p = NULL;

			int i = np2(size);
			//DBLOCK(Log("[StringPool] Alloc %d, np2 = %d", size, i));

			switch (i)
			{
				default:
					if (i > 32)
						return NULL;

				case 32:
					FIND_POOL(32);
					//DBLOCK(Log("[StringPool] get 32 0x%lx", (long int)p));
				break;

				case 64:
					FIND_POOL(64);
					//DBLOCK(Log("[StringPool] get 64 0x%lx", (long int)p));
				break;

				case 128:
					FIND_POOL(128);
					//DBLOCK(Log("[StringPool] get 128 0x%lx", (long int)p));
				break;

				case 256:
					FIND_POOL(256);
					//DBLOCK(Log("[StringPool] get 256 0x%lx", (long int)p));
				break;

				case 512:
					FIND_POOL(512);
					//DBLOCK(Log("[StringPool] get 512 0x%lx", (long int)p));
				break;

				case 1024:
					FIND_POOL(1024);
					//DBLOCK(Log("[StringPool] get 1024 0x%lx", (long int)p));
				break;

				case 2048:
					FIND_POOL(2048);
					//DBLOCK(Log("[StringPool] get 2048 0x%lx", (long int)p));
				break;

				case 4096:
					FIND_POOL(4096);
					//DBLOCK(Log("[StringPool] get 4096 0x%lx", (long int)p));
				break;
			}

			iTotalCount++;
			iCurrentCount++;

			return p;
		}

		void PrintSnapshot()
		{
			DBLOCK(
				Log("[StringPool] Total Allocated: %d", iTotalCount);
				Log("[StringPool] Total in use...: %d", iCurrentCount);

				PRINT_POOL(32);
				PRINT_POOL(64);
				PRINT_POOL(128);
				PRINT_POOL(256);
				PRINT_POOL(512);
				PRINT_POOL(1024);
				PRINT_POOL(2048);
				PRINT_POOL(4096);
			)
		}

		void LeakReport()
		{
			DBLOCK(
				CHECK_POOL(32, true);
				CHECK_POOL(64, true);
				CHECK_POOL(128, true);
				CHECK_POOL(256, true);
				CHECK_POOL(512, true);
				CHECK_POOL(1024, true);
				CHECK_POOL(2048, true);
				CHECK_POOL(4096, true);
			)
		}

		void OverRunReport()
		{
			DBLOCK(
				CHECK_POOL(32, false);
				CHECK_POOL(64, false);
				CHECK_POOL(128, false);
				CHECK_POOL(256, false);
				CHECK_POOL(512, false);
				CHECK_POOL(1024, false);
				CHECK_POOL(2048, false);
				CHECK_POOL(4096, false);
			)
		}

	private:
		SEED_DISABLE_COPY(StringPoolManager<T>);

	private:
		DEFINE_POOL(32);
		DEFINE_POOL(64);
		DEFINE_POOL(128);
		DEFINE_POOL(256);
		DEFINE_POOL(512);
		DEFINE_POOL(1024);
		DEFINE_POOL(2048);
		DEFINE_POOL(4096);
		u32 iTotalCount;
		u32 iCurrentCount;
};

} // namespace

#endif // SEED_USE_STRING_POOL

#endif // __STRING_POOL_H__

/* Sample usage - Implement unity test */
/*
	fprintf(stdout, "Pool size: %ld\n", (unsigned long int)sizeof(glStringPool));

	fprintf(stdout, "+ Alloc:\n");
	char *a = glStringPool.Alloc(1);
	char *b = glStringPool.Alloc(32);
	char *c = glStringPool.Alloc(33);
	char *d = glStringPool.Alloc(60);
	char *e = glStringPool.Alloc(64);
	char *f = glStringPool.Alloc(65);
	char *g = glStringPool.Alloc(120);
	char *h = glStringPool.Alloc(128);
	char *i = glStringPool.Alloc(129);

	strcpy(a, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	strcpy(b, "bbbbbbbbbb");
	strcpy(c, "ccccccccccccccccccccccc");
	strcpy(i, "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");

	fprintf(stdout, "+ Snapshot:\n");
	glStringPool.PrintSnapshot();

	fprintf(stdout, "+ Overrun:\n");
	glStringPool.OverRunReport(); //LeakReport();

	fprintf(stdout, "+ Freeing:\n");
	glStringPool.Free(a);
	glStringPool.Free(b);
	glStringPool.Free(c);
	glStringPool.Free(d);
	glStringPool.Free(e);
	glStringPool.Free(f);
	glStringPool.Free(g);
	glStringPool.Free(h);
	glStringPool.Free(i);

	fprintf(stdout, "+ Leaks:\n");
	glStringPool.LeakReport();
*/
