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

/*! \file IMemoryManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Manager class interface
*/

#include "interface/IMemoryManager.h"
#include "interface/IMemoryPool.h"
#include "Log.h"

namespace Seed {

IMemoryManager::IMemoryManager()
{
	arPool.Truncate();
}

IMemoryManager::~IMemoryManager()
{
}

INLINE IMemoryPool *IMemoryManager::CreatePool(u32 len, const char *name)
{
	UNUSED(len);
	UNUSED(name);

	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE u32 IMemoryManager::GetFreeMemory() const
{
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE void *IMemoryManager::Alloc(SIZE_T len, IMemoryPool *pool, const char *desc, const char *owner)
{
	UNUSED(len);
	UNUSED(pool);
	UNUSED(desc);
	UNUSED(owner);
	SEED_ABSTRACT_METHOD;

	return NULL;
}

INLINE void IMemoryManager::Free(void *ptr, IMemoryPool *pool)
{
	UNUSED(ptr);
	UNUSED(pool);
	SEED_ABSTRACT_METHOD;
}

INLINE void IMemoryManager::Reserve(SIZE_T len, IMemoryPool *pool)
{
	UNUSED(len);
	UNUSED(pool);
	SEED_ABSTRACT_METHOD;
}

INLINE void IMemoryManager::Unreserve(SIZE_T len, IMemoryPool *pool)
{
	UNUSED(len);
	UNUSED(pool);
	SEED_ABSTRACT_METHOD;
}

INLINE void IMemoryManager::Info()
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IMemoryManager::DisableThreadCache()
{
	for (u32 i = 0; i < arPool.Size(); i++)
	{
		IMemoryPool *pool = arPool[i];
		pool->DisableThreadCache();
	}
}

INLINE BOOL IMemoryManager::IsRequired() const
{
	return TRUE;
}

INLINE const char *IMemoryManager::GetObjectName() const
{
	return "MemoryManager";
}

} // namespace
