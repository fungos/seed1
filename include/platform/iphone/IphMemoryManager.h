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

/*! \file IphMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager Iphone Implementation
*/

#ifndef __IPH_MEMORY_MANAGER_H__
#define __IPH_MEMORY_MANAGER_H__

#if defined(_IPHONE_)

#include <stdlib.h>
#include "interface/IMemoryManager.h"
#include "platform/iphone/IphMemoryPool.h"
#include "Singleton.h"

namespace Seed { namespace iPhone {

class MemoryManager : public IMemoryManager
{
	SEED_SINGLETON_DECLARE(MemoryManager);
	public:
		virtual u32 GetFreeMemory() const;

		virtual void *Alloc(SIZE_T len, IMemoryPool *pool = &defaultPool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool = &defaultPool);
		virtual void Info();

		//virtual u32 GetFreeMemory() const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static IphoneMemoryPool defaultPool;

	private:
		SEED_DISABLE_COPY(MemoryManager);
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(MemoryManager);
SEED_CORE_API extern IphoneMemoryPool *const pDefaultPool = &MemoryManager::defaultPool;
}

#define pLargePool pDefaultPool

}} // namespace

#else // _IPHONE_
	#error "Include 'MemoryManager.h' instead 'platform/iphone/IphMemoryManager.h' directly."
#endif // _IPHONE_
#endif // __MEMORY_MANAGER__
