/*! \file IEventListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event listener class interface
*/


#ifndef __IEVENT_LISTENER_H__
#define __IEVENT_LISTENER_H__


#include "Defines.h"


namespace Seed {


class IEventListener
{
	public:
		IEventListener();
		virtual ~IEventListener();

	private:
		SEED_DISABLE_COPY(IEventListener);
};


} // namespace


#endif // __IEVENT_LISTENER_H__
