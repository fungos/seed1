/*! \file CollisionMask.cpp
	\author Danny Angelo Carminati Grein
	\brief Collision Mask to be do fast pixel test on sprites (ie. Button)
*/


#include "CollisionMask.h"
#include "Formats.h"


namespace Seed {


IResource *CollisionMaskResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	CollisionMask *mask = new CollisionMask();
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

INLINE void *CollisionMask::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

INLINE void CollisionMask::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}

INLINE const char *CollisionMask::GetObjectName() const
{
	return "CollisionMask";
}

INLINE int CollisionMask::GetObjectType() const
{
	return Seed::ObjectCollisionMask;
}


} // namespace

