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

/*! \file PackageManager.cpp
	\author Everton Fernando Patitucci da Silva
	\brief Package system
*/

#include "PackageManager.h"
#include "Log.h"
#include "Package.h"

#define TAG		"[PackageManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(PackageManager);

PackageManager::PackageManager()
	: mapPackage()
{
}

PackageManager::~PackageManager()
{
	this->Clear();
}

void PackageManager::Clear()
{
	PackageMapIterator itb = mapPackage.begin();
	PackageMapIterator ite = mapPackage.end();

	for (; itb != ite; ++itb)
	{
		Package *pkg = (*itb).second;

		Log(TAG "Removing package %s.", (*itb).first);
		sRelease(pkg);
	}

	mapPackage.clear();
	PackageMap().swap(mapPackage);
}

BOOL PackageManager::Reset()
{
	this->Clear();
	return TRUE;
}

BOOL PackageManager::Shutdown()
{
	this->Reset();
	return IModule::Shutdown();
}

void PackageManager::Remove(const char *fileName)
{
	PackageMapIterator it = mapPackage.find(fileName);
	if (it != mapPackage.end())
	{
		Package *p = (*it).second;
		sRelease(p);

		mapPackage.erase(it);
	}
}

void PackageManager::AddRomPackage(const void *addr, const char *name)
{
	ASSERT_NULL(addr);

	Package *p = New(Package(name));
	p->LockUnload();
	p->Load(addr);

	Log(TAG "Adding package ROM.package.");
	mapPackage[name] = p;
}

void PackageManager::Add(const char *fileName, IMemoryPool *pool, ResourceManager *res)
{
	ASSERT_NULL(fileName);
	ASSERT_NULL(pool);
	ASSERT_NULL(res);

	Package *p = static_cast<Package *>(res->Get(fileName, Seed::ObjectPackage, pool));
	ASSERT_NULL(p);

	Log(TAG "Adding package %s.", fileName);
	mapPackage[fileName] = p;
}

const void *PackageManager::GetFile(const char *fileName, u32 *fileSize)
{
	const void *file = NULL;

	PackageMapIterator itb = mapPackage.begin();
	PackageMapIterator ite = mapPackage.end();

	for (; itb != ite; ++itb)
	{
		Package *p = (*itb).second;
		file = p->GetFile(fileName, fileSize);
		if (file)
			break;
	}

	return file;
}

INLINE const char *PackageManager::GetObjectName() const
{
	return "PackageManager";
}

} // namespace
