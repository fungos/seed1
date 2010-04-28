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

/*! \file IMemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Memory Manager class interface
*/

#ifndef __IMEMORYMANAGER_H__
#define __IMEMORYMANAGER_H__

#include "IModule.h"

namespace Seed {

class IMemoryPool;

/// MemoryManager Interface
/**
*/
class SEED_CORE_API IMemoryManager : public IModule
{
	public:
		IMemoryManager();
		virtual ~IMemoryManager();

		virtual void *Alloc(SIZE_T len, IMemoryPool *pool, const char *desc = "Unknown", const char *owner = "Nobody");
		virtual void Free(void *ptr, IMemoryPool *pool);
		virtual void Info();

		virtual u32 GetFreeMemory() const;

		// IModule
		virtual BOOL IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

	private:
		SEED_DISABLE_COPY(IMemoryManager);
};

} // namespace

#endif // __IMEMORYMANAGER_H__
