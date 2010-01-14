/*! \file EventInputPointer.cpp
	\author	Danny Angelo Carminati Grein
	\brief A event from an input pointer
*/

#include "EventInputPointer.h"

namespace Seed {


EventInputPointer::EventInputPointer(u32 j, u32 pressed, u32 hold, u32 released, f32 x, f32 y)
	: IEventInput(EVENT_INPUT_POINTER)
	, iJoystick(j)
	, iPressed(pressed)
	, iHold(hold)
	, iReleased(released)
	, fX(x)
	, fY(y)
{
}


EventInputPointer::~EventInputPointer()
{
}


INLINE u32 EventInputPointer::GetJoystick() const
{
	return iJoystick;
}


INLINE u32 EventInputPointer::GetPressed()  const
{
	return iPressed;
}


INLINE u32 EventInputPointer::GetReleased() const
{
	return iReleased;
}


INLINE u32 EventInputPointer::GetHold() const
{
	return iHold;
}


INLINE f32 EventInputPointer::GetX() const
{
	return fX;
}


INLINE f32 EventInputPointer::GetY() const
{
	return fY;
}


INLINE const char *EventInputPointer::GetObjectName() const
{
	return "EventInputPointer";
}


} // namespace

