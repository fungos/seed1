/*! \file CollisionMask.h
	\author Danny Angelo Carminati Grein
	\brief Collision Mask to be do fast pixel test on sprites (ie. Button)
*/

#ifndef __COLLISION_MASK_H__
#define __COLLISION_MASK_H__


#include "interface/IResource.h"
#include "MemoryManager.h"
#include "SeedInit.h"
#include "File.h"


namespace Seed {


IResource *CollisionMaskResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);


class CollisionMask : public IResource
{
	friend IResource *CollisionMaskResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

	public:
		CollisionMask();
		virtual ~CollisionMask();

		virtual BOOL CheckPixel(u32 x, u32 y) const;

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [] (void *);

	private:
		SEED_DISABLE_COPY(CollisionMask);

	private:
		File		stFile;
		u16			iWidth;
		u16			iHeight;
		const u8	*pData;
};


} // namespace


#endif // __COLLISION_MASK_H__
