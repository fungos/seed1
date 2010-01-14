/*! \file ResourceGroup.h
	\author	Rafael Eduardo Gonchor
	\brief Defines the resource group class
*/


#ifndef __RESOURCEGROUP_H__
#define __RESOURCEGROUP_H__

#include "Log.h"
#include "interface/IEvent.h"
#include "interface/IEventListener.h"

#include "MemoryManager.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "SeedInit.h"

#include <vector>
#include <algorithm>


namespace Seed {


class ResourceGroup
{
	friend class ResourceLoader;


	public:
		ResourceGroup();
		virtual ~ResourceGroup();

		void Add(const char *filename, Seed::eObjectType resourceType = Seed::ObjectSprite, IMemoryPool *pool = pDefaultPool, ResourceManager *res = &glResourceManager);


	protected:
		typedef struct QueueItem
		{
			const char 			*filename;
			IResource			*resource;
			Seed::eObjectType 	resourceType;
			IMemoryPool			*pool;
			ResourceManager		*resManager;
			u32					startTime;
			BOOL				erased;
		} QueueItem;

		typedef std::vector<QueueItem *> QueueVector;
		typedef QueueVector::iterator	QueueIterator;

	protected:
		BOOL Load();
		BOOL Unload();

		void SetLoaded();
		BOOL IsLoaded() const;


	protected:
		QueueVector		queue;
		BOOL			bLoaded;


	private:
		SEED_DISABLE_COPY(ResourceGroup);
};


} // namespace


#endif // __RESOURCEGROUP_H__
