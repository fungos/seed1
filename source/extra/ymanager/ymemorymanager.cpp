/*
Author.: Danny Angelo Carminati Grein (danny.cabelo@gmail.com)
Copyright (c) 2009 all rights reserved.
Description:

	This is a debugging memory allocator to analyze a program memory use,
	by no means this meant to be optimized and used in a production environment.
*/

#include "extra/ymanager/ymemorymanager.h"

/*
	memory layout:

		DEBUG              RELEASE
		|.-28|| -24|| -20|-| -16|| -12||  -8||  -4||0 ...   n|
		[KEEP][OWNR][DESC]-[PREV][NEXT][USED][SIZE][user area]
		u32   str   str   -u32   u32   u32   u32   ?

struct block
{
	u32			keep;
	const char *ownr;
	const char *desc;
	u32			prev;
	u32			next;
	u32			used;
	u32			size;
};
*/

#if defined(YMEM_DEBUG)
#define KEEP(i)			(*((u32 *)(&this->pPool[i - 28])))
#define OWNR(i)			(*((u32 *)(&this->pPool[i - 24])))
#define DESC(i)			(*((u32 *)(&this->pPool[i - 20])))
#define SIZE_HEADER		32
#else
#define KEEP(i)
#define OWNR(i)
#define DESC(i)
#define SIZE_HEADER		16
#endif // YMEM_DEBUG

#define NO_OWNR		"Nobody"
#define NO_DESC		"Empty"

#define UNK_OWNR	"Unknown"
#define UNK_DESC	"Unknown"

#define START		SIZE_HEADER		/*address of first payload*/

#define PREV(i)		(*((u32 *)(&this->pPool[i - 16])))
#define NEXT(i)		(*((u32 *)(&this->pPool[i - 12])))
#define USED(i)		(*((u32 *)(&this->pPool[i -  8])))
#define SIZE(i)		(*((u32 *)(&this->pPool[i -  4])))

#define GUARD		0xdeadbeef

#define TAG "[YMemoryManager] "

YMemoryManager::YMemoryManager(u32 nbytes)
	: pPool(NULL)
	, iSize(nbytes)
	, iFreeMemory(0)
{
	this->pPool = (unsigned char *)malloc(nbytes);
	//this->iSize = nbytes;

	if (this->iSize <= SIZE_HEADER)
	{
		LOG0(TAG "Not enough memory to initialize memory manager.\n");
		exit(1);
	}

	_YDEBUG
	(
		KEEP(START) = GUARD;
		OWNR(START) = (PTRSIZE)NO_OWNR;
		DESC(START) = (PTRSIZE)NO_DESC;
		this->iOverrunCount = 0;
		this->iAllocationCount = 0;
		this->iFreeCount = 0;
		this->iAllocationTotalBytes = 0;
	)
	PREV(START) = 0;
	NEXT(START) = 0;
	USED(START) = FALSE;
	SIZE(START) = this->iSize - SIZE_HEADER;
	this->iFreeMemory = SIZE(START);

}

YMemoryManager::~YMemoryManager()
{
	free(this->pPool);

	this->iSize = 0;
	this->iFreeMemory = 0;
	this->pPool = NULL;

	_YDEBUG
	(
		this->iOverrunCount = 0;
		this->iAllocationCount = 0;
		this->iFreeCount = 0;
		this->iAllocationTotalBytes = 0;
	)

}


void *YMemoryManager::Alloc(u32 len, const char *desc, const char *owner)
{
	LOG1(TAG "Alloc(%d)\n", len);

	if (len == 0)
		return NULL;

	// find a empty block
	u32 current = START;
	while (NEXT(current) != 0)
	{
		if ((SIZE(current) >= len) && USED(current) == FALSE)
		{
			this->Split(current, len, desc, owner);
			_YDEBUG
			(
				this->iAllocationCount++;
				this->iAllocationTotalBytes += len;
			)
			return ((void *)&this->pPool[current]);
		}
		current = NEXT(current);
	}

	// handle the last block NEXT(current) = 0
	if ((SIZE(current) >= len) && !USED(current))
	{
		this->Split(current, len, desc, owner);
		_YDEBUG
		(
			this->iAllocationCount++;
			this->iAllocationTotalBytes += len;
		)
		return ((void *)&this->pPool[current]);
	}

	return NULL;
}


void YMemoryManager::Free(void *addr)
{
#if defined(YMEM_PARANOID_CHECK)
	if (addr == NULL)
	{
		LOG0(TAG "Trying to free NULL pointer.\n");
		return;
	}

	LOG1(TAG "Free(0x%x).\n", (int)addr);

	if ((addr >= (void *)&this->pPool[this->iSize]) || (addr < (void *)&this->pPool[0]))
	{
		LOG0(TAG "Address not in manager pool.\n");
		return;
	}
#endif // YMEM_PARANOID_CHECK

	u32 free = (u32)(((u8 *)addr) - this->pPool);
	LOG1(TAG "Amount to be freed: %d.\n", SIZE(free));

#if defined(YMEM_PARANOID_CHECK)
	if (free < SIZE_HEADER)
		 return;

	// check if someone invaded our block
	_YDEBUG
	(
		if (KEEP(free) != GUARD)
		{
			//u32 prev = PREV(free);
			LOG2(TAG "WARNING: this block was invaded by %s (%s).\n", (const char *)DESC(PREV(free)), (const char *)OWNR(prev));
			this->iOverrunCount++;
		}
	)

	if ((USED(free) != TRUE)			||	// region is already free
		(PREV(free) >= free)			||	// previous element is not previous
		(NEXT(free) >= this->iSize)		||	// next is outside of our pool
		(SIZE(free) >= this->iSize)		||	// size region greater than our pool
		(SIZE(free) == 0))					// invalid size
	{
		LOG0(TAG "Referencing invalid address.\n");
		return;
	}
#endif // YMEM_PARANOID_CHECK

	_YDEBUG
	(
		DESC(free) = (PTRSIZE)NO_DESC;
		OWNR(free) = (PTRSIZE)NO_OWNR;
		this->iFreeCount++;
	)
	this->Merge(PREV(free), free, NEXT(free));
}


/*
split a [free] region into [alloc][free] pair if possible
*/
void YMemoryManager::Split(u32 addr, u32 len, const char *desc, const char *owner)
{
	UNUSED(desc);
	UNUSED(owner);
	/*
	need enough space for:
		len = size of request
		SIZE_HEADER = block for a new region
		SIZE_HEADER = minimun space for a new region
	*/

	if (SIZE(addr) >= len + SIZE_HEADER + SIZE_HEADER)
	{
		this->iFreeMemory -= len + SIZE_HEADER;

		u32 oldnext = NEXT(addr);
		u32 oldprev = PREV(addr);
		u32 oldsize = SIZE(addr);
		u32 newaddr = addr + len + SIZE_HEADER;

		_YDEBUG
		(
			KEEP(addr) = GUARD;
			OWNR(addr) = (PTRSIZE)(!owner ? UNK_OWNR : owner);
			DESC(addr) = (PTRSIZE)(!desc  ? UNK_DESC : desc);
		)
		NEXT(addr) = newaddr;
		PREV(addr) = oldprev;
		USED(addr) = TRUE;
		SIZE(addr) = len;

		_YDEBUG
		(
			KEEP(newaddr) = GUARD;
			OWNR(newaddr) = (PTRSIZE)NO_OWNR;
			DESC(newaddr) = (PTRSIZE)NO_DESC;
		)
		NEXT(newaddr) = oldnext;
		PREV(newaddr) = addr;
		USED(newaddr) = FALSE;
		SIZE(newaddr) = oldsize - len - SIZE_HEADER;
	}
	else
	{
		this->iFreeMemory -= SIZE(addr);
		USED(addr) = TRUE;
		_YDEBUG
		(
			KEEP(addr) = GUARD;
			OWNR(addr) = (PTRSIZE)(!owner ? UNK_OWNR : owner);
			DESC(addr) = (PTRSIZE)(!desc  ? UNK_DESC : desc);
		)
	}
}

/*
	   4 cases (F = free / O = occupied)
		   FOF -> [F]
		   OOF -> O[F]
		   FOO -> [F]O
		   OOO -> OFO
*/
void YMemoryManager::Merge(u32 prev, u32 current, u32 next)
{
	/*
	first handle special cases of region at end(s)
		prev = 0				low end
		next = 0				high end
		prev = 0 and next = 0	only 1 list element
	*/
	if (!prev)
	{
		if (!next)
		{
			USED(current) = FALSE;
			this->iFreeMemory += SIZE(current);
		}
		else if (USED(next) == TRUE)
		{
			USED(current) = FALSE;
			this->iFreeMemory += SIZE(current);
		}
		else if (USED(next) == FALSE)
		{
			u32 temp;

			LOG0(TAG "Merging current block with next block.\n");
			this->iFreeMemory += SIZE(current) + SIZE_HEADER;
			USED(current) = FALSE;
			SIZE(current) = SIZE(current) + SIZE(next) + SIZE_HEADER;
			NEXT(current) = NEXT(next);

			temp = NEXT(next);
			PREV(temp) = current;
		}
	}
	else if (next == 0)
	{
		if (USED(prev) == TRUE)
		{
			USED(current) = FALSE;
			this->iFreeMemory += SIZE(current);
		}
		else if (USED(prev) == FALSE)
		{
			LOG0(TAG "Merging current block with previous block.\n");
			this->iFreeMemory += SIZE(current) + SIZE_HEADER;
			SIZE(prev) = SIZE(prev) + SIZE(current) + SIZE_HEADER;
			NEXT(prev) = NEXT(current);
		}
	}

	/* now we handle 4 cases */
	else if ((USED(prev) == TRUE) && (USED(next) == TRUE))
	{
		USED(current) = FALSE;
		this->iFreeMemory += SIZE(current);
	}
	else if ((USED(prev) == TRUE) && (USED(next) == FALSE))
	{
		u32 temp;

		LOG0(TAG "Merging current block with next block.\n");
		this->iFreeMemory += SIZE(current) + SIZE_HEADER;
		USED(current) = FALSE;
		SIZE(current) = SIZE(current) + SIZE(next) + SIZE_HEADER;
		NEXT(current) = NEXT(next);

		temp = NEXT(next);
		if (temp != 0)
		{
			PREV(temp) = current;
		}
	}
	else if ((USED(prev) == FALSE) && (USED(next) == TRUE))
	{
		LOG0(TAG "Merging current block with previous block.\n");
		this->iFreeMemory += SIZE(current) + SIZE_HEADER;
		SIZE(prev) = SIZE(prev) + SIZE(current) + SIZE_HEADER;
		NEXT(prev) = NEXT(current);
		PREV(next) = prev;
	}
	else if ((USED(prev) == FALSE) && (USED(next) == FALSE))
	{
		u32 temp;

		LOG0(TAG "Merging current block with previous and next blocks.\n");
		this->iFreeMemory += SIZE(current) + SIZE_HEADER + SIZE_HEADER;
		SIZE(prev) = SIZE(prev) +
					SIZE(current) + SIZE_HEADER +
					SIZE(next) + SIZE_HEADER;
		NEXT(prev) = NEXT(next);

		temp = NEXT(next);
		if (temp != 0)
		{
			PREV(temp) = prev;
		}
	}
}

u32 YMemoryManager::GetFreeMemory()
{
	return this->iFreeMemory;
}

#if defined(YMEM_DEBUG)
void YMemoryManager::PrintSnapshot()
{
	u32 i;
	u32 current;

	i = 0;
	current = START;

	printf(TAG "Snapshot:\n");
	while (NEXT(current) != 0)
	{
		printf("\tBLOCK %d: [Addr=%8d][Size=%8d][%s]", i, current, SIZE(current), USED(current) ? "Busy" : "Free");
		printf("[%s][%s]\n", (const char *)OWNR(current), (const char *)DESC(current));
		current = NEXT(current);
		i++;
	}

	printf("\tBLOCK %d: [Addr=%8d][Size=%8d][%s]", i, current, SIZE(current), USED(current) ? "Busy" : "Free");
	printf("[%s][%s]\n", (const char *)OWNR(current), (const char *)DESC(current));
	printf(TAG "Free Memory: %d\n", this->iFreeMemory);
	printf(TAG "Allocations: %d\n", this->iAllocationCount);
	printf(TAG "Free'd     : %d\n", this->iFreeCount);
	printf(TAG "Overruns   : %d\n", this->iOverrunCount);
	printf(TAG "Average len: %.2f\n", this->iAllocationTotalBytes / (double)this->iAllocationCount);
}

u32 YMemoryManager::GetOverrunCount()
{
	return this->iOverrunCount;
}

u32 YMemoryManager::GetAllocationCount()
{
	return this->iAllocationCount;
}

u32 YMemoryManager::GetFreeCount()
{
	return this->iFreeCount;
}

u32 YMemoryManager::GetAllocationTotalBytes()
{
	return this->iAllocationTotalBytes;
}

#endif // YMEM_DEBUG
