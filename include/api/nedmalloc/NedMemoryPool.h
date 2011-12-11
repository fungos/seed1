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

/*! \file NedMemoryPool.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryPool implementation using nedmalloc
*/

#ifndef __NED_MEMORY_POOL_H__
#define __NED_MEMORY_POOL_H__

#include "Defines.h"

#if SEED_USE_NEDMALLOC == 1

#include <stdlib.h>
#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"
#include "../contrib/nedmalloc/nedmalloc.h"

namespace Seed {

#if defined(_PC_)
namespace PC { class MemoryManager; }
#define MemoryManagerFriendship friend class PC::MemoryManager;
#else
#define MemoryManagerFriendship friend class MemoryManager;
#endif

class SEED_CORE_API NedMemoryPool : public IMemoryPool
{
	MemoryManagerFriendship

	public:
		NedMemoryPool(u32 size, const char *name);
		virtual void *Alloc(SIZE_T len, const char *desc, const char *owner);
		virtual void Free(void *ptr);
		virtual u32 GetFreeMemory() const;
		virtual void DisableThreadCache();
		virtual void Print() const;

	protected:
		virtual void Initialize();
		virtual ~NedMemoryPool();

	private:
		SEED_DISABLE_COPY(NedMemoryPool);

	private:
		nedpool *pPool;
};

} // namespace

#endif // SEED_USE_NEDMALLOC
#endif // __NED_MEMORY_POOL_H__
