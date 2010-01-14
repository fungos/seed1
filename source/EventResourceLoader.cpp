/*! \file EventResourceLoader.cpp
	\author	Rafael Eduardo Gonchor
	\brief A event from resource loader
*/


#include "EventResourceLoader.h"


namespace Seed {


EventResourceLoader::EventResourceLoader()
	: IEvent(RESOURCE, EVENT_RESOURCE_LOADER)
	, pGroup(FALSE)
{
}


EventResourceLoader::~EventResourceLoader()
{
}


INLINE void EventResourceLoader::SetGroup(ResourceGroup *group)
{
	this->pGroup = group;
}


INLINE ResourceGroup *EventResourceLoader::GetGroup() const
{
	return this->pGroup;
}


INLINE const char *EventResourceLoader::GetObjectName() const
{
	return "EventResourceLoader";
}


} // namespace
