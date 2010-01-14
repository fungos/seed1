/*! \file EventResourceLoader.h
	\author	Rafael Eduardo Gonchor
	\brief A event from resource loader
*/


#ifndef __EVENTRESOURCE_LOADER_H__
#define __EVENTRESOURCE_LOADER_H__


#include "interface/IEvent.h"


namespace Seed {


class ResourceGroup;


class EventResourceLoader : public IEvent
{
	public:
		EventResourceLoader();
		virtual ~EventResourceLoader();

		void SetGroup(ResourceGroup *pGroup);
		ResourceGroup *GetGroup() const;

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		ResourceGroup *pGroup;

	private:
		SEED_DISABLE_COPY(EventResourceLoader);
};


} // namespace


#endif // __EVENTRESOURCE_LOADER_H__
