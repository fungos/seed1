/*! \file IInputJoystick.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Joystick Input class interface
*/


#include "interface/IInputJoystick.h"


namespace Seed {


IInputJoystick::IInputJoystick()
{
}

IInputJoystick::~IInputJoystick()
{
}

INLINE BOOL IInputJoystick::IsHold(u32 button, u16 joystick)
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL IInputJoystick::IsPressed(u32 button, u16 joystick)
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL IInputJoystick::IsReleased(u32 button, u16 joystick)
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE f32 IInputJoystick::GetX(u16 joystick)
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE f32 IInputJoystick::GetY(u16 joystick)
{
	UNUSED(joystick);
	return 0.0f;
}


} // namespace

