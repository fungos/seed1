/*! \file IEventInputMotionListener.h
	\author	Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief Defines the event motion listener class interface
*/


#ifndef __IEVENT_INPUT_MOTION_LISTENER_H__
#define __IEVENT_INPUT_MOTION_LISTENER_H__


#include "interface/IEventListener.h"
#include "EventInputMotion.h"


namespace Seed {


class IEventInputMotionListener : public IEventListener
{
	public:
		IEventInputMotionListener();
		virtual ~IEventInputMotionListener();

		virtual void OnAccelerationChanged(const EventInputMotion *ev);

	private:
		SEED_DISABLE_COPY(IEventInputMotionListener);
};


} // namespace


#endif // __IEVENT_INPUT_POINTER_LISTENER_H__

