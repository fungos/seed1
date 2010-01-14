/*! \file ISound.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Sound class interface
*/

#ifndef __ISOUND_H__
#define __ISOUND_H__


#include "SeedInit.h"
#include "IResource.h"
#include "MemoryManager.h"
#include "ResourceManager.h"


namespace Seed {


class ISound : public IResource
{
	public:
		ISound();
		virtual ~ISound();

		virtual const void *GetData() const = 0;

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool) = 0;
		virtual BOOL Unload() = 0;

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [] (void *);

	private:
		SEED_DISABLE_COPY(ISound);
};


} // namespace

#endif // __ISOUND_H__
