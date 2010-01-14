/*! \file IEventResourceLoaderListener.h
	\author Rafael Eduardo Gonchor
	\brief Defines the resource loader event listener class interface
*/


#ifndef __IEVENTRESOURCELOADERLISTENER_H__
#define __IEVENTRESOURCELOADERLISTENER_H__


#include "interface/IEventListener.h"


namespace Seed {


class EventResourceLoader;


class IEventResourceLoaderListener : public IEventListener
{
	public:
		IEventResourceLoaderListener();
		~IEventResourceLoaderListener();

		virtual void OnGroupLoaded(const EventResourceLoader *ev);
		virtual void OnQueueEmpty(const EventResourceLoader *ev);

	private:
		SEED_DISABLE_COPY(IEventResourceLoaderListener);
};


} // namespace


#endif // __IEVENTRESOURCELOADERLISTENER_H__
