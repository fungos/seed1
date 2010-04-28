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

/*! \file PcMemoryPool.cpp
	\author	Danny Angelo Carminati Grein
	\brief Memory Pool PC implementation
*/

#include "MemoryPool.h"

#if defined(_PC_)

#if defined(YMEM_DEBUG)
	#include "extra/ymanager/yallocwrap.h"

	#define SEED_MALLOC(len, desc, owner)	(void *)yalloc(this->pMgr, len, desc, owner)
	#define SEED_FREE(ptr)					yfree(this->pMgr, ptr)
	#define YMANAGER_CREATE(len)			this->pMgr = ycreate(len)
	#define YMANAGER_DESTROY				ydestroy(this->pMgr); this->pMgr = NULL;
	#define YMANAGER_GETFREEMEM				if (this->pMgr) return this->pMgr->GetFreeMemory(); else return 0;
	#define YMANAGER_PRINT					this->pMgr->PrintSnapshot();
#else
	#define SEED_MALLOC(len, desc, owner)	(void *)malloc(len)
	#define SEED_FREE(ptr)					free(ptr)
	#define YMANAGER_CREATE(len)
	#define YMANAGER_DESTROY
	#define YMANAGER_GETFREEMEM				return 0;
	#define YMANAGER_PRINT
#endif // DEBUG

namespace Seed { namespace PC {

PcMemoryPool::PcMemoryPool(u32 size)
	YMANAGER_INIT
{
	UNUSED(size);
	YMANAGER_CREATE(size);
}

PcMemoryPool::~PcMemoryPool()
{
	YMANAGER_DESTROY;
}

INLINE void PcMemoryPool::Initialize()
{
}

INLINE u32 PcMemoryPool::GetFreeMemory() const
{
	YMANAGER_GETFREEMEM;
	//return 0;
}

INLINE void PcMemoryPool::Print() const
{
	YMANAGER_PRINT;
}

void *PcMemoryPool::Alloc(SIZE_T len, const char *desc, const char *owner)
{
	UNUSED(owner);
	UNUSED(desc);
	iAllocations++;
	iTotalAllocations++;
	return SEED_MALLOC((unsigned long)len, desc, owner);
	//return NULL;
}

void PcMemoryPool::Free(void *ptr)
{
	iAllocations--;
	SEED_FREE(ptr);
}

DefaultMemoryPool::DefaultMemoryPool(u32 size)
	: PcMemoryPool(size)
{
}

DefaultMemoryPool::~DefaultMemoryPool()
{
}

LargeMemoryPool::LargeMemoryPool(u32 size)
	: PcMemoryPool(size)
{
}

LargeMemoryPool::~LargeMemoryPool()
{
}

}} // namespace

#endif // _PC_
