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

/*! \file PackageManager.h
	\author Everton Fernando Patitucci da Silva
	\brief Package system
*/

#ifndef __PACKAGEMANAGER_H__
#define __PACKAGEMANAGER_H__

#include "Base.h"
#include "MemoryManager.h"
#include "SeedInit.h"
#include "interface/IFileSystem.h"
#include "Singleton.h"

#include <map>

namespace Seed {

class ResourceManager;
class Package;

class SEED_CORE_API PackageManager : public IModule
{
	friend class IFileSystem;
	SEED_SINGLETON_DECLARE(PackageManager);

	public:
		typedef std::map<const char *, Package *, LowerThanStringComparator> PackageMap;
		typedef std::map<const char *, Package *, LowerThanStringComparator>::iterator PackageMapIterator;

	public:
		void AddRomPackage(const void *addr, const char *name);
		void Add(const char *fileName, IMemoryPool *pool = pDefaultPool, ResourceManager *res = pResourceManager);
		void Remove(const char *fileName);
		void Clear();

		// IModule
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		const void *GetFile(const char *fileName, u32 *fileSize);

	private:
		SEED_DISABLE_COPY(PackageManager);

	private:
		PackageMap 	mapPackage;
};

//extern "C" {
//SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(PackageManager);
//}

#define pPackageManager PackageManager::GetInstance()

} // namespace

#endif // __PACKAGEMANAGER_H__
