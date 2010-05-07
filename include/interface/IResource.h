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

/*! \file IResource.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Resource object interface
*/

#ifndef __IRESOURCE_H__
#define __IRESOURCE_H__

#include "Defines.h"
#include "IObject.h"

#define sAcquire(p)		p->Acquire()
#define sRelease(p)		{ if (p) p->Release(); p = NULL; }

namespace Seed {

class ResourceManager;
class IMemoryPool;

class SEED_CORE_API IResource : public IObject
{
	friend class ResourceManager;

	public:
		IResource();
		virtual ~IResource();

		virtual BOOL Load(const char *filename);
		virtual BOOL Load(const char *filename, IMemoryPool *pool);
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool) = 0;
		virtual BOOL Unload() = 0;

		virtual u32 GetUsedMemory() const;

		void Release();
		void Acquire();

	protected:
		void IncrementReference();
		void DecrementReference();

		u32 GetReferenceCount() const;

	protected:
		BOOL			bLoaded;
		IMemoryPool		*pPool;
		ResourceManager *pRes;
		const char		*pFilename;

	private:
		SEED_DISABLE_COPY(IResource);

	private:
		u32				iRefCount;
};

} // namespace

#endif // __IRESOURCE_H__
