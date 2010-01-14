/*! \file EventInputMotion.cpp
	\author	Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief A event from an input motion
*/

#include "EventInputMotion.h"

namespace Seed {


EventInputMotion::EventInputMotion(u32 j, Vector3f ptAcc, f32 fMag, f32 fVel)
	: IEventInput(EVENT_INPUT_MOVEMENT)
	, iJoystick(j)
	, fMagnitude(fMag)
	, fSpeed(fVel)
{
	ptAcceleration = ptAcc;
}


EventInputMotion::~EventInputMotion()
{
}

INLINE u32 EventInputMotion::GetJoystick() const
{
	return iJoystick;
}

INLINE Vector3f EventInputMotion::GetAccelerationAxis() const
{
	return ptAcceleration;
}


INLINE f32 EventInputMotion::GetAccelerationMagnitude()  const
{
	return fMagnitude;
}


INLINE f32 EventInputMotion::GetAccelerationSpeed() const
{
	return fSpeed;
}

INLINE const char *EventInputMotion::GetObjectName() const
{
	return "EventInputMotion";
}


} // namespace

