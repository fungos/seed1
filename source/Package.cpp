/*! \file Package.cpp
	\author Everton Fernando Patitucci da Silva
	\brief Package system
*/


#include "Package.h"
#include "FileSystem.h"
#include "Formats.h"
#include "Log.h"


#define TAG		"[Package] "


namespace Seed {


struct PackageFSTable
{
	u32 offset;
	u32 fileSize;
	// fileName
};

IResource *PackageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Package *pkg = new Package();
	pkg->Load(filename, res, pool);

	return pkg;
}

Package::Package()
	: stFile()
	, iFilesAmount(0)
{
}

Package::Package(const char *name)
	: stFile()
	, iFilesAmount(0)
{
	stFile.SetName(name);
}

Package::~Package()
{
	this->Unload();
}

INLINE u32 Package::GetUsedMemory() const
{
	return IResource::GetUsedMemory() + this->stFile.GetSize() + sizeof(this);
}

BOOL Package::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(pool);
	ASSERT_NULL(res);
	ASSERT_NULL(filename);

	this->pFilename = filename;
	SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Could not open package file.");

	return this->Load(stFile.GetData(), res, pool);
}

BOOL Package::Load(const void *data, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);
	UNUSED(pool);

	this->pPool = pool;
	this->pRes = res;

	//BOOL result = FALSE;
	const u8 *ptr = static_cast<const u8 *>(data);

	stFile.SetData(data);
	ObjectHeader *block = NULL;
	READ_STRUCT(block, ObjectHeader, ptr);

	SECURITY_CHECK(seed_validate_block(&stFile, block, PACKAGE_OBJECT_MAGIC, PACKAGE_OBJECT_VERSION), "Invalid block header for font.");
	READ_U32(iFilesAmount, ptr);

	if (!iFilesAmount)
	{
		Log(TAG "Package %s is empty.", stFile.GetName());
		pFileSystem->Close(&stFile);
		bLoaded = FALSE;
	}
	else
	{
		bLoaded = TRUE;
	}

	return bLoaded;
}

BOOL Package::Unload()
{
	if (stFile.GetData())
	{
		pFileSystem->Close(&stFile);
	}

	return TRUE;
}

const void *Package::GetFile(const char *filename, u32 *filesize)
{
	const void *file = NULL;

	const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
	SKIP_STRUCT(ObjectHeader, ptr);
	u32 foo;
	READ_U32(foo, ptr);

	for (u32 i = 0; i < iFilesAmount; i++)
	{
		u32 offset = 0;
		u32 size = 0;
		char *name = NULL;

		READ_U32(offset, ptr);
		READ_U32(size, ptr);
		READ_STR(name, ptr);

		if (!STRCMP(name, filename))
		{
			const u8 *filePtr = static_cast<const u8 *>(stFile.GetData());
			file = &filePtr[offset];

			*filesize = size;
			break;
		}
	}

	return file;
}

INLINE const char *Package::GetObjectName() const
{
	return "Package"; //this->stFile.GetName();
}

INLINE int Package::GetObjectType() const
{
	return Seed::ObjectPackage;
}

INLINE void Package::LockUnload()
{
	stFile.bPackaged = TRUE; // FIXME: Hackerish so the package marked Locked won't be freed - ie.: ROM
}

void *Package::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

void Package::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}


} // namespace
