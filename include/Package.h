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


class Package : public IResource
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
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Load(const void *data, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [](void *);

	private:
		Package();
		SEED_DISABLE_COPY(Package);

	private:
		File				stFile;
		u32					iFilesAmount;
};


} // namespace


#endif // __PACKAGE_H__

