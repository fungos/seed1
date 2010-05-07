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

/*! \file IphMemoryManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "MemoryManager.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/sysctl.h>

#define TAG "[MemoryManager] "

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(MemoryManager);
IphoneMemoryPool MemoryManager::defaultPool;

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
}

BOOL MemoryManager::Reset()
{
	return TRUE;
}

BOOL MemoryManager::Initialize()
{
	Log(TAG "Initializing...");

	BOOL r = this->Reset();
	Log(TAG "Initialization completed. Free Memory: %d.", this->GetFreeMemory());
	this->Info();

	return r;
}

BOOL MemoryManager::Shutdown()
{
	Log(TAG "Terminating...");
	BOOL r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

INLINE u32 MemoryManager::GetFreeMemory() const
{
	return 0;
}

INLINE void *MemoryManager::Alloc(SIZE_T len, IMemoryPool *pool, const char *desc, const char *owner)
{
	//IphoneMemoryPool *p = static_cast<IphoneMemoryPool *>(pool);
	return (void *)malloc(ROUND_UP(len, 32));
}

INLINE void MemoryManager::Free(void *ptr, IMemoryPool *pool)
{
	free(ptr);
}

INLINE void MemoryManager::Info()
{
}

}} // namespace

#endif // _IPHONE_
