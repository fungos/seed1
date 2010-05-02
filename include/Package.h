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

/*! \file Package.h
	\author Everton Fernando Patitucci da Silva
	\brief Package system
*/

#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "Base.h"
#include "interface/IResource.h"
#include "MemoryManager.h"
#include "File.h"
#include "SeedInit.h"

#include <map>

namespace Seed {

class ResourceManager;
class IResource;

IResource *PackageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

class SEED_CORE_API Package : public IResource
{
	friend IResource *PackageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class PackageManager;

	public:
		Package(const char *name);
		~Package();

		// IResource
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		const void *GetFile(const char *filename, u32 *filesize);
		void LockUnload();

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Load(const void *data, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		SEED_DISABLE_INSTANCING;

	private:
		Package();
		SEED_DISABLE_COPY(Package);

	private:
		File				stFile;
		u32					iFilesAmount;
};

} // namespace

#endif // __PACKAGE_H__

