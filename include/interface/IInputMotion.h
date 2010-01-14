/*! \file IInputMotion.h
	\author	Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief Defines Input Motion class interface
*/

#ifndef __IINPUT_MOTION_H__
#define __IINPUT_MOTION_H__

#include "interface/IInput.h"
#include "Array.h"
#include "Vector.h"


namespace Seed {


class EventInputMotion;
class IEventInputMotionListener;


/// Pointer Input Interface
/**
Platforms that support Accelerometer like input must implement this interface.
*/
class IInputMotion
{
	public:
		IInputMotion();
		virtual ~IInputMotion();

		virtual Vector3f GetAccelerationAxis(u16 joystick = 0) const;
		virtual f32 GetAccelerationMagnitude(u16 joystick = 0) const;
		virtual f32 GetAccelerationSpeed(u16 joystick = 0) const;

		void AddMotionListener(IEventInputMotionListener *listener);
		void RemoveMotionListener(IEventInputMotionListener *listener);

	protected:
		void SendEventAccelerationChanged(const EventInputMotion *ev);

	protected:
		Array<IEventInputMotionListener *, 1000> arMotionListeners;

	private:
		SEED_DISABLE_COPY(IInputMotion);
};


} // namespace


#endif // __IINPUT_POINTER_H__

