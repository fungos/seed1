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

/*! \file NedMemoryPool.cpp
	\author	Danny Angelo Carminati Grein
	\brief Memory Pool PC implementation using nedmalloc
*/

#include "Defines.h"

#if SEED_USE_NEDMALLOC == 1

#include "api/nedmalloc/NedMemoryPool.h"

#define TAG "[NedMemoryPool] "

namespace Seed { 

NedMemoryPool::NedMemoryPool(u32 size, const char *name)
	: pPool(NULL)
{
	pcName = name;
	pPool = nedcreatepool(size, 8);
	//nedpsetvalue(pPool, 0xF0DAFOD4);
}

NedMemoryPool::~NedMemoryPool()
{
	nedflushlogs(pPool, 0);
	neddestroypool(pPool);
}

INLINE void NedMemoryPool::Initialize()
{
}

INLINE u32 NedMemoryPool::GetFreeMemory() const
{
	return 0;
}

INLINE void NedMemoryPool::Print() const
{
	Log(TAG "Name: %s", pcName);
}

void *NedMemoryPool::Alloc(SIZE_T len, const char *desc, const char *owner)
{
	UNUSED(owner);
	UNUSED(desc);
	iAllocations++;
	iTotalAllocations++;

	return nedpmemalign(pPool, sizeof(int), len);
}

void NedMemoryPool::DisableThreadCache()
{
	neddisablethreadcache(pPool);
}

void NedMemoryPool::Free(void *ptr)
{
	iAllocations--;

	nedpfree(pPool, ptr);
}

} // namespace

#endif // SEED_USE_NEDMALLOC
