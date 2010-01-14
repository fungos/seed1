/*! \file PackageManager.cpp
	\author Everton Fernando Patitucci da Silva
	\brief Package system
*/


#include "PackageManager.h"
#include "Log.h"
#include "Package.h"


#define TAG		"[PackageManager] "


namespace Seed {


PackageManager PackageManager::instance;

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
		pkg->Release();
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
		p->Release();
		//glResourceManager.GarbageCollect();

		mapPackage.erase(it);
	}
}


void PackageManager::AddRomPackage(const void *addr, const char *name)
{
	ASSERT_NULL(addr);

	Package *p = new Package(name);
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


void *PackageManager::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}


void PackageManager::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}


} // namespace
