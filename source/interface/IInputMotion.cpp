/*! \file IInputMotion.cpp
	\author	Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief Defines Input Motion class interface
*/


#include "interface/IInputMotion.h"
#include "interface/IEventInputMotionListener.h"
#include "Log.h"
#include "Array.h"

#include <vector>
#include <algorithm>


namespace Seed {


IInputMotion::IInputMotion()
	: arMotionListeners()
{
}

IInputMotion::~IInputMotion()
{
}

INLINE Vector3f IInputMotion::GetAccelerationAxis(u16 joystick) const
{
	UNUSED(joystick);
	return Vector3f(0.0f, 0.0f, 0.0f);
}

INLINE f32 IInputMotion::GetAccelerationMagnitude(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE f32 IInputMotion::GetAccelerationSpeed(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE void IInputMotion::AddMotionListener(IEventInputMotionListener *listener)
{
	ASSERT_NULL(listener);
	arMotionListeners.Add(listener);
}

INLINE void IInputMotion::RemoveMotionListener(IEventInputMotionListener *listener)
{
	ASSERT_NULL(listener);
	for (u32 i = 0; i < arMotionListeners.Size(); i++)
	{
		if (arMotionListeners[i] == listener)
		{
			arMotionListeners.Del(i);
			break;
		}
	}
}

INLINE void IInputMotion::SendEventAccelerationChanged(const EventInputMotion *ev)
{
	ASSERT_NULL(ev);

	for (u32 i = 0; i < arMotionListeners.Size(); i++)
	{
		ASSERT_NULL(arMotionListeners[i]);
		arMotionListeners[i]->OnAccelerationChanged(ev);
	}
}


} // namespace


