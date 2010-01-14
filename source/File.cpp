/*! \file EventSystem.cpp
	\author	Everton Fernando Patitucci da Silva
	\brief A system event
*/


#include "Defines.h"
#include "MemoryManager.h"
#include "File.h"
#include "FileSystem.h"
#include "Log.h"

#define TAG		"[File] "


namespace Seed {

File::File()
	: pName(NULL)
	, pData(NULL)
	, pPool(NULL)
	, iSize(0)
	, bPackaged(FALSE)
{
}


File::File(const char *filename, IMemoryPool *pool)
	: pName(filename)
	, pData(NULL)
	, pPool(pool)
	, iSize(0)
	, bPackaged(FALSE)
{
	pFileSystem->Open(filename, this, pool);
	// FIXME:
	// cyclic dependency not good, not good
	// file don't need to know about filesystem.. but, this will make the usage simplier.
}


File::~File()
{
	this->Close();
}


void File::Close()
{
	if (!bPackaged && pData)
	{
		pMemoryManager->Free(const_cast<void *>(pData), pPool);
	}

	pName = NULL;
	pData = NULL;
	pPool = NULL;
	iSize = 0;
	bPackaged = FALSE;
}


void *File::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}


void File::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}


INLINE void File::SetSize(u32 size)
{
	iSize = size;
}


INLINE u32 File::GetSize() const
{
	return iSize;
}

INLINE void File::SetData(const void *data)
{
	pData = data;
}

INLINE const void *File::GetData() const
{
	return pData;
}


INLINE IMemoryPool *File::GetPool() const
{
	return pPool;
}


INLINE const char *File::GetName() const
{
	return pName;
}


INLINE void File::SetName(const char *name)
{
	pName = name;
}


} // namespace
