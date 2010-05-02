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

struct SEED_CORE_API PackageFSTable
{
	u32 offset;
	u32 fileSize;
	// fileName
};

IResource *PackageResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	Package *pkg = New(Package());
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

SEED_DISABLE_INSTANCING_IMPL(Package);

} // namespace
