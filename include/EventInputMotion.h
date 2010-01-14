/*! \file EventInputMotion.h
	\author	Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief A event from an input pointer
*/

#ifndef __EVENT_INPUT_MOTION_H__
#define __EVENT_INPUT_MOTION_H__


#include "interface/IEventInput.h"
#include "Vector.h"


namespace Seed {



class EventInputMotion : public IEventInput
{
	public:
		EventInputMotion(u32 j, Vector3f ptAcc, f32 fMag, f32 fVel);
		virtual ~EventInputMotion();

		u32 GetJoystick() const;
		Vector3f GetAccelerationAxis() const;
		f32 GetAccelerationMagnitude()  const;
		f32 GetAccelerationSpeed() const;

		// IObject
		const char *GetObjectName() const;

	protected:
		Vector3f ptAcceleration;
		u32 iJoystick;
		f32 fMagnitude;
		f32 fSpeed;

	private:
		SEED_DISABLE_COPY(EventInputMotion);
};


} // namespace


#endif // __EVENT_INPUT_MOTION_H__

