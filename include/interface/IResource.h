/*! \file IResource.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Resource object interface
*/

#ifndef __IRESOURCE_H__
#define __IRESOURCE_H__


#include "Defines.h"
#include "IObject.h"


namespace Seed {


class ResourceManager;
class IMemoryPool;


class IResource : public IObject
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

