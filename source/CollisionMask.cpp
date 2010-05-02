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

/*! \file CollisionMask.cpp
	\author Danny Angelo Carminati Grein
	\brief Collision Mask to be do fast pixel test on sprites (ie. Button)
*/

#include "CollisionMask.h"
#include "Formats.h"

namespace Seed {

IResource *CollisionMaskResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	CollisionMask *mask = New(CollisionMask());
	mask->Load(filename, res, pool);

	return mask;
}

CollisionMask::CollisionMask()
	: stFile()
	, iWidth(0)
	, iHeight(0)
	, pData(NULL)
{
}

CollisionMask::~CollisionMask()
{
}

INLINE BOOL CollisionMask::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);
	ASSERT_NULL(filename);
	ASSERT_NULL(res);
	ASSERT_NULL(pool);

	if (this->Unload())
	{
		this->pRes = res;
		this->pPool = pool;
		this->pFilename = filename;

		/* Isso deveria ser um objeto como SpriteObject -> ButtonObject, para que o .button seja reaproveitado. */
		SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Could not open collision mask file.");

		const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
		ObjectHeader *block = NULL;
		READ_STRUCT(block, ObjectHeader, ptr);
		SECURITY_CHECK(seed_validate_block(&stFile, block, MASK_OBJECT_MAGIC, MASK_OBJECT_VERSION), "Invalid block header for collision mask.");

		READ_U16(this->iWidth, ptr);
		READ_U16(this->iHeight, ptr);

		this->pData = static_cast<const u8 *>(ptr);
		this->bLoaded = TRUE;
	}
	return bLoaded;
}

INLINE BOOL CollisionMask::Unload()
{
	stFile.Close();
	this->bLoaded	= FALSE;
	this->iWidth	= 0;
	this->iHeight	= 0;
	this->pData		= NULL;

	return TRUE;
}

INLINE BOOL CollisionMask::CheckPixel(u32 x, u32 y) const
{
	u32 bitPos = (x % 8);
	u32 bytePosX = (x / 8);
	u32 byteTest = static_cast<u32>(1 << (7 - bitPos));

	u32 byte = this->pData[y * iWidth + bytePosX];

	return ((byte & byteTest) == byteTest);
}

INLINE u32 CollisionMask::GetUsedMemory() const
{
	return sizeof(this) + stFile.GetSize();
}

INLINE const char *CollisionMask::GetObjectName() const
{
	return "CollisionMask";
}

INLINE int CollisionMask::GetObjectType() const
{
	return Seed::ObjectCollisionMask;
}

SEED_DISABLE_INSTANCING_IMPL(CollisionMask);

} // namespace
