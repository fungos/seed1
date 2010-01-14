/*! \file ParticleEmitterObject.cpp
	\author	Danny Angelo Carminati Grein
	\brief A particle emitter configuration file
*/

#include "ParticleEmitterObject.h"
#include "Enum.h"


namespace Seed {


IResource *ParticleEmitterObjectResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ParticleEmitterObject *obj = new ParticleEmitterObject();
	obj->Load(filename, res, pool);

	return obj;
}

ParticleEmitterObject::ParticleEmitterObject()
	: iMemory(0)
{
}

ParticleEmitterObject::~ParticleEmitterObject()
{
	this->Unload();
}

INLINE BOOL ParticleEmitterObject::Unload()
{
	pFileSystem->Close(&stFile);
	return TRUE;
}

INLINE BOOL ParticleEmitterObject::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pFilename = filename;
		pRes 	= res;
		pPool 	= pool;
		iMemory = 0;

		SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Particle Emitter couldn't be opened");
		//u8 *ptr = const_cast<u8 *>(static_cast<const u8 *>(stFile.GetData()));
		//ObjectHeader *block = NULL;
		//READ_STRUCT(block, ObjectHeader, ptr);
		//SECURITY_CHECK(seed_validate_block(&stFile, block, SPRITE_OBJECT_MAGIC, SPRITE_OBJECT_VERSION), "Invalid block header for sound.");

		iMemory += stFile.GetSize() + sizeof(this);

		bLoaded = TRUE;
	}

	return bLoaded;
}

INLINE const ParticleEmitterInfo *ParticleEmitterObject::GetData() const
{
	return reinterpret_cast<const ParticleEmitterInfo *>(stFile.GetData());
}

INLINE u32 ParticleEmitterObject::GetUsedMemory() const
{
	return iMemory;
}

INLINE const char *ParticleEmitterObject::GetObjectName() const
{
	return "ParticleEmitterObject";
}

INLINE int ParticleEmitterObject::GetObjectType() const
{
	return Seed::ObjectParticleEmitterObject;
}

void *ParticleEmitterObject::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

void ParticleEmitterObject::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}

} // namespace
