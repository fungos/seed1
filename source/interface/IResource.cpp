/*! \file IResource.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Resource object interface
*/


#include "interface/IResource.h"
#include "SeedInit.h"
#include "MemoryManager.h"


namespace Seed {


IResource::IResource()
	: bLoaded(FALSE)
	, pPool(NULL)
	, pRes(NULL)
	, pFilename(NULL)
	, iRefCount(0)
{
}

IResource::~IResource()
{
}

INLINE void IResource::Release()
{
	this->DecrementReference();
}

INLINE void IResource::Acquire()
{
	this->IncrementReference();
}

INLINE u32 IResource::GetUsedMemory() const
{
	return sizeof(this);
}

INLINE void IResource::IncrementReference()
{
	this->iRefCount++;
}

INLINE void IResource::DecrementReference()
{
	this->iRefCount--;
}

INLINE u32 IResource::GetReferenceCount() const
{
	return iRefCount;
}

INLINE BOOL IResource::Load(const char *filename, IMemoryPool *pool)
{
	return this->Load(filename, &glResourceManager, pool);
}

INLINE BOOL IResource::Load(const char *filename)
{
	return this->Load(filename, &glResourceManager, pDefaultPool);
}
/*
INLINE BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	this->pFilename	= filename;
	this->pRes		= res;
	this->pPool		= pool;
}
*/

} // namespace

