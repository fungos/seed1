/*! \file IEventInputMotionListener.cpp
	\author	Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief Defines the event motion listener class interface
*/

#include "interface/IEventInputMotionListener.h"


namespace Seed {


IEventInputMotionListener::IEventInputMotionListener()
{
}

IEventInputMotionListener::~IEventInputMotionListener()
{
}

void IEventInputMotionListener::OnAccelerationChanged(const EventInputMotion *ev)
{
	UNUSED(ev);
}


} // namespace

