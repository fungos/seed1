/*! \file EventFileSystem.cpp
	\author	Rafael Eduardo Gonchor
	\brief A file system event
*/


#include "EventFileSystem.h"


namespace Seed {


EventFileSystem::EventFileSystem()
	: IEvent(SYSTEM, EVENT_FILE_SYSTEM)
	, pFilename(NULL)
{
}

EventFileSystem::~EventFileSystem()
{
}

INLINE void EventFileSystem::SetFile(const char *pFilename)
{
	this->pFilename = pFilename;
}

INLINE const char *EventFileSystem::GetFile()
{
	return this->pFilename;
}

INLINE const char *EventFileSystem::GetObjectName() const
{
	return "EventFileSystem";
}


} // namespace
