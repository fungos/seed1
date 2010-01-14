/*! \file ResourceManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Resource Manager
*/


#include "Defines.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "Base.h"

#define TAG		"[ResourceManager] "


#if defined(SEED_LOG_RESOURCEMANAGER)
	#define LOG		Log
#else
	#define LOG		_hid_rmlog
	void _hid_rmlog(const char *pMessage, ...);
	void _hid_rmlog(const char *pMessage, ...)
	{
		UNUSED(pMessage);
	}
#endif


namespace Seed {


LoaderMap ResourceManager::mapLoaders;


ResourceManager::ResourceManager(const char *name)
	: bHasUnusedResource(FALSE)
	, pcName(name)
	, mapResources()
{
}

ResourceManager::~ResourceManager()
{
	if (mapResources.size())
	{
		LOG(TAG "WARNING: Some resources still allocated in '%s'.", pcName);
		this->Print();
	}

	this->Reset();
}

INLINE void ResourceManager::Reset()
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	for (; it != itEnd; ++it)
	{
		//LOG(TAG "Deallocating %s.", (*it).first);
		delete (*it).second;
	}

	mapResources.clear();
	ResourceMap().swap(mapResources);
}

IResource *ResourceManager::Get(const char *filename, Seed::eObjectType resourceType, IMemoryPool *pool)
{
	IResource *res = NULL;

	if (mapResources.find(filename) == mapResources.end())
	{
		LOG(TAG "Resource %s not found in '%s'.", filename, pcName);
		LoaderIterator it = mapLoaders.find(resourceType);
		if (it == mapLoaders.end())
		{
			LOG(TAG "Resource loader for %s not found.", filename);
			return NULL;
		}

		res = ((*it).second)(filename, this, pool);
		this->Add(filename, res);
	}
	else
	{
		res = mapResources[filename];
	}

	res->IncrementReference();
	return res;
}

void ResourceManager::GarbageCollect()
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	LOG(TAG "Starting garbage collection in '%s'...", pcName);

#ifdef DEBUG
	u64 begin = pTimer->GetMilliseconds();
	u64 end = 0;
#endif // DEBUG

	for (; it != itEnd;)
	{
		IResource *res = (*it).second;

		u32 r = res->GetReferenceCount();
		if (!r)
		{
			LOG(TAG "\tdeleting %s from [%s].", (*it).first, pcName);
			mapResources.erase(it++);
			delete res;

			bHasUnusedResource = TRUE;
		}
		else
		{
			++it;
		}
	}

/*
FIXME: 30-10-2008 | BUG | MAJOR | CORE | Resources sendo liberados que liberarem resources iram deixar resources sem referencia. Workaround para remover. | Danny Angelo Carminati Grein
*/
	if (bHasUnusedResource)
	{
		bHasUnusedResource = FALSE;
		this->GarbageCollect();
	}

#ifdef DEBUG
	end = pTimer->GetMilliseconds();
	LOG(TAG "Garbage collection done in %d milliseconds.",  (u32)(end - begin));
	LOG(TAG "Resources inside '%s': ", pcName);
	this->Print();
#endif // DEBUG
}

INLINE void ResourceManager::Unload(Seed::eObjectType resourceType)
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	//u32 total = 0;
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		if (res->GetObjectType() == resourceType)
		{
			LOG(TAG "Unloading %s %s.", res->GetObjectName(), (*it).first);
			res->Unload();
		}
	}
}

INLINE void ResourceManager::Reload(Seed::eObjectType resourceType)
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	//u32 total = 0;
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		if (res->GetObjectType() == resourceType)
		{
			LOG(TAG "Reloading %s %s.", res->GetObjectName(), (*it).first);
			res->Load(res->pFilename, res->pRes, res->pPool);
		}
	}
}

INLINE void ResourceManager::Register(Seed::eObjectType resourceType, pResourceLoaderFunc pfunc)
{
	if (mapLoaders.find(resourceType) != mapLoaders.end())
	{
		LOG(TAG "This resource loader is already registered.");
		return;
	}

	mapLoaders[resourceType] = pfunc;
}

INLINE void ResourceManager::Unregister(Seed::eObjectType resourceType)
{
	LoaderIterator it = mapLoaders.find(resourceType);

	if (it == mapLoaders.end())
	{
		LOG(TAG "Resource loader not found.");
		return;
	}

	mapLoaders.erase(it);
}

INLINE void ResourceManager::Add(const char *filename, IResource *res)
{
	if (mapResources.find(filename) != mapResources.end())
	{
		LOG(TAG "The resource %s already is allocated in '%s'.", filename, pcName);
		return;
	}

	mapResources[filename] = res;
}

INLINE void ResourceManager::Remove(const char *filename)
{
	ResourceIterator it = mapResources.find(filename);

	if (it == mapResources.end())
	{
		LOG(TAG "Resource %s not found in '%s'.", filename, pcName);
		return;
	}

	mapResources.erase(it);
}

u32 ResourceManager::GetTotalUsedMemory()
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	u32 total = 0;
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		total += res->GetUsedMemory();
	}

	return total;
}

INLINE void ResourceManager::Print()
{
#if defined(DEBUG) && defined(SEED_LOG_RESOURCEMANAGER)
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	u32 cnt = 0;
	Log(TAG "Listing %d loaded resources in '%s':", mapResources.size(), pcName);
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;
		const char *name = (*it).first;

		Log(TAG "\t%s [%s] [%d]", name, res->GetObjectName(), res->GetReferenceCount());
		cnt++;
	}

	Log(TAG "%s Total: %d resources.", pcName, cnt);
#endif // DEBUG
}


} // namespace


