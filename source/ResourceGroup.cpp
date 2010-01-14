/*! \file ResourceGroup.cpp
	\author	Rafael Eduardo Gonchor
	\brief Resource group class
*/

#include "ResourceGroup.h"


namespace Seed {


ResourceGroup::ResourceGroup()
	: queue()
	, bLoaded(FALSE)
{
}


ResourceGroup::~ResourceGroup()
{
	queue.clear();
	std::vector<QueueItem *>().swap( queue );
}


void ResourceGroup::Add(const char *filename, Seed::eObjectType resourceType, IMemoryPool *pool, ResourceManager *res)
{
	QueueItem *pNewItem 	= new QueueItem();
	pNewItem->filename 		= filename;
	pNewItem->resource 		= NULL;
	pNewItem->resourceType 	= resourceType;
	pNewItem->resManager	= res;
	pNewItem->pool 			= pool;
	pNewItem->startTime		= (u32)pTimer->GetMilliseconds();
	pNewItem->erased		= FALSE;

	queue.push_back(pNewItem);
}


BOOL ResourceGroup::Load()
{
	QueueIterator it 	= queue.begin();
	QueueIterator end 	= queue.end();

	for (; it != end; ++it)
	{
		QueueItem *pQueueItem = (*it);

		//already loaded
		if (pQueueItem->resource)
			continue;

		ASSERT_NULL(pQueueItem);

		IResource *res;
		res = pQueueItem->resManager->Get(pQueueItem->filename, pQueueItem->resourceType, pQueueItem->pool);

		pQueueItem->resource = res;
	}

	return TRUE;
}


BOOL ResourceGroup::Unload()
{
	QueueIterator it 	= queue.begin();
	QueueIterator end 	= queue.end();

	for (; it != end; ++it)
	{
		QueueItem *pQueueItem = (*it);

		ASSERT_NULL(pQueueItem);

		//already loaded
		if (pQueueItem->resource)
		{
			pQueueItem->resource->Release();
		}

		delete pQueueItem;
	}

	queue.clear();
	QueueVector().swap(queue);

	return TRUE;
}


INLINE void ResourceGroup::SetLoaded()
{
	this->bLoaded = TRUE;
}


INLINE BOOL ResourceGroup::IsLoaded() const
{
	return this->bLoaded;
}


} // namespace

