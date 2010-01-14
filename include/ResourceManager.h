/*! \file ResourceManager.h
	\author	Danny Angelo Carminati Grein
	\brief Resource Manager
*/

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Config.h"
#include "interface/IResource.h"
#include "interface/IMemoryPool.h"
#include "MemoryManager.h"
#include "Base.h"
#include "Enum.h"

#include <map>


namespace Seed {


typedef IResource *(*pResourceLoaderFunc)(const char *filename, ResourceManager *res, IMemoryPool *pool);

typedef std::map<const char *, IResource *, LowerThanStringComparator> ResourceMap;
typedef ResourceMap::iterator ResourceIterator;

typedef std::map<Seed::eObjectType, pResourceLoaderFunc> LoaderMap;
typedef LoaderMap::iterator LoaderIterator;


class ResourceManager
{
	public:
		ResourceManager(const char *name);
		~ResourceManager();

		void Reset();
		IResource *Get(const char *filename, Seed::eObjectType resourceType = Seed::ObjectImage, IMemoryPool *pool = pDefaultPool);
		void GarbageCollect();

		u32 GetTotalUsedMemory();

		static void Register(Seed::eObjectType resourceType, pResourceLoaderFunc pfunc);
		static void Unregister(Seed::eObjectType resourceType);

		void Unload(Seed::eObjectType resourceType);
		void Reload(Seed::eObjectType resourceType);
		void Print();

	private:
		SEED_DISABLE_COPY(ResourceManager);

		void Add(const char *filename, IResource *res);
		void Remove(const char *filename);

	private:
		BOOL bHasUnusedResource;

		const char *pcName;

		ResourceMap mapResources;
		static LoaderMap mapLoaders;
};


} // namespace


#endif // __RESOURCE_MANAGER_H__


