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

void *ResourceManager::mapLoaders[ObjectUser];

ResourceManager::ResourceManager(const char *name)
	: bHasUnusedResource(FALSE)
	, pcName(name)
{
	MEMSET(mapLoaders, 0, sizeof(mapLoaders));
	MEMSET(mapResources, 0, sizeof(mapResources));
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			LOG(TAG "WARNING: Some resources still allocated in '%s'.", pcName);
			this->Print();
			break;
		}
	}

	this->Reset();
}

INLINE void ResourceManager::Reset()
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			const char *first = mapResources[i].pcKey;

			LOG(TAG "Deallocating %s.", first);
			Delete(second);
		}
	}

	MEMSET(mapResources, 0, sizeof(mapResources));
}

IResource *ResourceManager::Get(const char *filename, Seed::eObjectType resourceType, IMemoryPool *pool)
{
	IResource *res = NULL;

	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pcKey && filename && STRCMP(mapResources[i].pcKey, filename) == 0)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			//const char *first = mapResources[i].pcKey;

			res = second;
			res->IncrementReference();
			//Log(TAG "Found allocated resource, incrementing reference counter to: %d.", res->GetReferenceCount());
			return res;
		}
	}

	if (mapLoaders[resourceType] != NULL)
	{
		pResourceLoaderFunc ldr = (pResourceLoaderFunc)mapLoaders[resourceType];
		res = (ldr)(filename, this, pool);
		if (res)
		{
			this->Add(filename, res);
			res->IncrementReference();
		}
		else
		{
			Log(TAG "WARNING: Resource file '%s' not found - couldn't load.", filename);
		}
	}

	return res;
}

void ResourceManager::GarbageCollect()
{
#if defined(DEBUG)
	u64 begin = pTimer->GetMilliseconds();
	u64 end = 0;
#endif // DEBUG

	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			const char *first = mapResources[i].pcKey;

			IResource *res = second;
			u32 r = res->GetReferenceCount();
			if (!r)
			{
				LOG(TAG "\tdeleting %s from [%s].", first, pcName);

				Delete(res);

				mapResources[i].pValue = NULL;
				mapResources[i].pcKey = NULL;

				bHasUnusedResource = TRUE;
			}
		}
	}

	if (bHasUnusedResource)
	{
		bHasUnusedResource = FALSE;
		this->GarbageCollect();
	}

#if defined(DEBUG)
	end = pTimer->GetMilliseconds();
	LOG(TAG "Garbage collection done in %d milliseconds.",  (u32)(end - begin));
	LOG(TAG "Resources inside '%s': ", pcName);
	this->Print();
#endif // DEBUG
}

INLINE void ResourceManager::Unload(Seed::eObjectType resourceType)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			const char *first = mapResources[i].pcKey;

			IResource *res = second;
			if (res->GetObjectType() == resourceType)
			{
				LOG(TAG "Unloading %s %s.", res->GetObjectName(), first);
				res->Unload();
			}
		}
	}
}

INLINE void ResourceManager::Reload(Seed::eObjectType resourceType)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			const char *first = mapResources[i].pcKey;

			IResource *res = second;
			if (res->GetObjectType() == resourceType)
			{
				LOG(TAG "Reloading %s %s.", res->GetObjectName(), first);
				res->Load(res->pFilename, res->pRes, res->pPool);
			}
		}
	}
}

INLINE void ResourceManager::Register(Seed::eObjectType resourceType, pResourceLoaderFunc pfunc)
{
	if (mapLoaders[resourceType])
	{
		LOG(TAG "This resource loader is already registered.");
		return;
	}

	mapLoaders[resourceType] = (void *)pfunc;
}

INLINE void ResourceManager::Unregister(Seed::eObjectType resourceType)
{
	if (mapLoaders[resourceType] == NULL)
	{
		LOG(TAG "Resource loader not found.");
		return;
	}

	mapLoaders[resourceType] = NULL;
}

INLINE void ResourceManager::Add(const char *filename, IResource *res)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pcKey && STRCMP(mapResources[i].pcKey, filename) == 0)
		{
			LOG(TAG "The resource %s already is allocated in '%s'.", filename, pcName);
			return;
		}
		else if (mapResources[i].pcKey == NULL)
		{
			LOG(TAG "Adding resource to position %d: %s: %s.", i, res->GetObjectName(), filename);
			mapResources[i].pcKey = filename;
			mapResources[i].pValue = (void *)res;
			return;
		}
	}

	LOG(TAG "Reached resource limit for '%s'.", pcName);
}

INLINE void ResourceManager::Remove(const char *filename)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pcKey && STRCMP(mapResources[i].pcKey, filename) == 0)
		{
			LOG(TAG "Removing resource '%s'.", filename);
			mapResources[i].pcKey = NULL;
			mapResources[i].pValue = NULL;
			return;
		}
	}

	LOG(TAG "Resource %s not found in '%s'.", filename, pcName);
}

void ResourceManager::PrintUsedMemoryByResource()
{
	u32 total = 0;
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			const char *first = mapResources[i].pcKey;

			IResource *res = second;

			Dbg(TAG "Resource: %s/%s Memory: %d References: %d Type: %s", first, res->pFilename, res->GetUsedMemory(), res->GetReferenceCount(), res->GetObjectName());
			total += res->GetUsedMemory();
		}
	}

	Dbg(TAG "Total: %d", total);
}

u32 ResourceManager::GetTotalUsedMemory()
{
	u32 total = 0;
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			IResource *res = second;
			total += res->GetUsedMemory();
		}
	}

	return total;
}

INLINE void ResourceManager::Print()
{
	u32 cnt = 0;
	Log(TAG "Listing loaded resources in '%s':", pcName);

	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *res = (IResource *)mapResources[i].pValue;
			const char *name = mapResources[i].pcKey;

			Log(TAG "\t%s [%s] [%d]", name, res->GetObjectName(), res->GetReferenceCount());
			cnt++;
		}
	}
	Log(TAG "%s Total: %d resources.", pcName, cnt);
}

} // namespace
