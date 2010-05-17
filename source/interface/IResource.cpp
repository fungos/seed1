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

/*! \file IResource.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Resource object interface
*/

#include "interface/IResource.h"
#include "SeedInit.h"
#include "MemoryManager.h"

namespace Seed {

IResource::IResource()
	: bLoaded(FALSE)
	, pPool(NULL)
	, pRes(NULL)
	, pFilename(NULL)
	, iRefCount(0)
{
}

IResource::~IResource()
{
}

INLINE void IResource::Release()
{
	this->DecrementReference();
}

INLINE void IResource::Acquire()
{
	this->IncrementReference();
}

INLINE u32 IResource::GetUsedMemory() const
{
	return sizeof(this);
}

INLINE void IResource::IncrementReference()
{
	this->iRefCount++;
}

INLINE void IResource::DecrementReference()
{
	this->iRefCount--;
}

INLINE u32 IResource::GetReferenceCount() const
{
	return iRefCount;
}

INLINE BOOL IResource::Load(const char *filename, IMemoryPool *pool)
{
	return this->Load(filename, pResourceManager, pool);
}

INLINE BOOL IResource::Load(const char *filename)
{
	return this->Load(filename, pResourceManager, pDefaultPool);
}
/*
INLINE BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	this->pFilename	= filename;
	this->pRes		= res;
	this->pPool		= pool;
}
*/

} // namespace

