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

/*! \file PcMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryManager PC Implementation
*/

#ifndef __PC_MEMORY_MANAGER_H__
#define __PC_MEMORY_MANAGER_H__

#include "Defines.h"

#if defined(_PC_)

#include <stdlib.h>
#include "LeakReport.h"
#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"
#include "MemoryPool.h"
#include "Singleton.h"

#define MB60	(u32)(59 * 1024 * 1024)
#define MB20	(u32)(19 * 1024 * 1024)
#define MB10	(u32)(13 * 1024 * 1024)

namespace Seed { namespace PC {

extern "C" {
SEED_CORE_API extern MemoryPool *pDefaultPool;
SEED_CORE_API extern MemoryPool *pLargePool;
}

class SEED_CORE_API MemoryManager : public IMemoryManager
{
	SEED_SINGLETON_DECLARE(MemoryManager)

	public:
		virtual void *Alloc(SIZE_T len, IMemoryPool *pool = pDefaultPool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool = pDefaultPool);
		virtual void Info();

		virtual IMemoryPool *CreatePool(u32 len, const char *name);
		virtual u32 GetFreeMemory() const;

		virtual void Reserve(SIZE_T len, IMemoryPool *pool);
		virtual void Unreserve(SIZE_T len, IMemoryPool *pool);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	private:
		SEED_DISABLE_COPY(MemoryManager);
};

#define pMemoryManager MemoryManager::GetInstance()

}} // namespace

#else // _PC_
	#error "Include 'MemoryManager.h' instead 'platform/pc/PcMemoryManager.h' directly."
#endif // _PC_
#endif // __PC_MEMORY_MANAGER_H__
