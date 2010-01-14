/*! \file IInputJoystick.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Joystick Input class interface
*/

#ifndef __IINPUT_JOYSTICK_H__
#define __IINPUT_JOYSTICK_H__


#include "IInput.h"


namespace Seed {


/// Joystick Input Interface
/**
Platforms that support Joystick must implement this interface.
*/
class IInputJoystick
{
	public:
		IInputJoystick();
		virtual ~IInputJoystick();

		virtual BOOL IsHold(u32 button, u16 joystick = 0);
		virtual BOOL IsPressed(u32 button, u16 joystick = 0);
		virtual BOOL IsReleased(u32 button, u16 joystick = 0);

		virtual f32 GetX(u16 joystick = 0);
		virtual f32 GetY(u16 joystick = 0);

	private:
		SEED_DISABLE_COPY(IInputJoystick);
};


} // namespace


#endif // __IINPUT_JOYSTICK_H__

