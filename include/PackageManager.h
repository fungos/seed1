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

#include <map>


namespace Seed {


class ResourceManager;
class Package;


class PackageManager : public IModule
{
	friend class IFileSystem;

	public:
		typedef std::map<const char *, Package *, LowerThanStringComparator> PackageMap;
		typedef std::map<const char *, Package *, LowerThanStringComparator>::iterator PackageMapIterator;


	public:
		PackageManager();
		virtual ~PackageManager();

		void AddRomPackage(const void *addr, const char *name);
		void Add(const char *fileName, IMemoryPool *pool = pDefaultPool, ResourceManager *res = &glResourceManager);
		void Remove(const char *fileName);
		void Clear();

		// IModule
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static PackageManager instance;


	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [](void *);

		const void *GetFile(const char *fileName, u32 *fileSize);


	private:
		SEED_DISABLE_COPY(PackageManager);

	private:
		PackageMap 	mapPackage;
};

PackageManager *const pPackageManager = &PackageManager::instance;


} // namespace


#endif // __PACKAGEMANAGER_H__

