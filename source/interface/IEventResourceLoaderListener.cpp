/*! \file IEventResourceLoaderListener.cpp
	\author Rafael Eduardo Gonchor
	\brief Defines the resource loader event listener class interface
*/

#include "Defines.h"
#include "interface/IEventResourceLoaderListener.h"

namespace Seed {

IEventResourceLoaderListener::IEventResourceLoaderListener()
{
}

IEventResourceLoaderListener::~IEventResourceLoaderListener()
{
}

void IEventResourceLoaderListener::OnGroupLoaded(const EventResourceLoader *ev)
{
	UNUSED(ev);
}

void IEventResourceLoaderListener::OnQueueEmpty(const EventResourceLoader *ev)
{
	UNUSED(ev);
}

} // namespace
