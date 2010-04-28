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

/*! \file PcMemoryPool.h
	\author	Danny Angelo Carminati Grein
	\brief MemoryPool PC Implementation
*/

#ifndef __PC_MEMORY_POOL_H__
#define __PC_MEMORY_POOL_H__

#include "Defines.h"

#if defined(_PC_)

#include <stdlib.h>
#include "interface/IMemoryPool.h"
#include "interface/IMemoryManager.h"
#include "extra/ymanager/ymemorymanager.h"

#if defined(DEBUG)
#define YMANAGER_INIT	: pMgr(NULL)
#define YMANAGER_VAR	YMemoryManager *pMgr
#else
#define YMANAGER_INIT
#define YMANAGER_VAR
#endif // DEBUG

namespace Seed { namespace PC {

class SEED_CORE_API PcMemoryPool : public IMemoryPool
{
	friend class MemoryManager;

	public:
		PcMemoryPool(u32 size);
		virtual void *Alloc(SIZE_T len, const char *desc, const char *owner);
		virtual void Free(void *ptr);
		virtual u32 GetFreeMemory() const;
		virtual void Print() const;

	protected:
		virtual void Initialize();
		virtual ~PcMemoryPool();

	private:
		SEED_DISABLE_COPY(PcMemoryPool);

	private:
		YMANAGER_VAR;
};

class SEED_CORE_API DefaultMemoryPool : public PcMemoryPool
{
	friend class MemoryManager;

	public:
		DefaultMemoryPool(u32 size);

	protected:
		virtual ~DefaultMemoryPool();

	private:
		SEED_DISABLE_COPY(DefaultMemoryPool);
};

class SEED_CORE_API LargeMemoryPool : public PcMemoryPool
{
	friend class MemoryManager;

	public:
		LargeMemoryPool(u32 size);

	protected:
		virtual ~LargeMemoryPool();

	private:
		SEED_DISABLE_COPY(LargeMemoryPool);
};

}} // namespace

#else // _PC_
	#error "Include 'MemoryManager.h' instead 'platform/pc/PcMemoryManager.h' directly."
#endif // _PC_
#endif // __PC_MEMORY_POOL_H__
