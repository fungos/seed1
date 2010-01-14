/*! \file IInputPointer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines Input Pointer class interface
*/


#include "interface/IInputPointer.h"
#include "interface/IEventInputPointerListener.h"
#include "Log.h"
#include "Array.h"

#include <vector>
#include <algorithm>


namespace Seed {


IInputPointer::IInputPointer()
	: arPointerListeners()
{
}

IInputPointer::~IInputPointer()
{
}

INLINE BOOL IInputPointer::IsHold(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL IInputPointer::IsPressed(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL IInputPointer::IsReleased(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE f32 IInputPointer::GetX(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE f32 IInputPointer::GetY(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE u32 IInputPointer::GetSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE void IInputPointer::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(sens);
	UNUSED(joystick);
}

INLINE u32 IInputPointer::GetMaximumSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE u32 IInputPointer::GetMaximumPointers() const
{
	return 0;
}

INLINE f32 IInputPointer::GetRelativeX(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE f32 IInputPointer::GetRelativeY(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE f32 IInputPointer::GetDistance(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE f32 IInputPointer::GetOrientation(u16 joystick) const
{
	UNUSED(joystick);
	return 0.0f;
}

INLINE BOOL IInputPointer::IsEnabled(u16 joystick) const
{
	UNUSED(joystick);
	return FALSE;
}

INLINE void IInputPointer::AddPointerListener(IEventInputPointerListener *listener)
{
	ASSERT_NULL(listener);
	arPointerListeners.Add(listener);
}

INLINE void IInputPointer::RemovePointerListener(IEventInputPointerListener *listener)
{
	ASSERT_NULL(listener);

	for (u32 i = 0; i < arPointerListeners.Size(); i++)
	{
		if (arPointerListeners[i] == listener)
		{
			arPointerListeners.Del(i);
			break;
		}
	}
}

INLINE void IInputPointer::SendEventPointerEnable(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	for (u32 i = 0; i < arPointerListeners.Size(); i++)
	{
		ASSERT_NULL(arPointerListeners[i]);
		arPointerListeners[i]->OnInputPointerEnable(ev);
	}
}

INLINE void IInputPointer::SendEventPointerDisable(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	for (u32 i = 0; i < arPointerListeners.Size(); i++)
	{
		ASSERT_NULL(arPointerListeners[i]);
		arPointerListeners[i]->OnInputPointerDisable(ev);
	}
}

INLINE void IInputPointer::SendEventPointerMove(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	for (u32 i = 0; i < arPointerListeners.Size(); i++)
	{
		ASSERT_NULL(arPointerListeners[i]);
		arPointerListeners[i]->OnInputPointerMove(ev);
	}
}

INLINE void IInputPointer::SendEventPointerPress(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	for (u32 i = 0; i < arPointerListeners.Size(); i++)
	{
		ASSERT_NULL(arPointerListeners[i]);
		arPointerListeners[i]->OnInputPointerPress(ev);
	}
}

INLINE void IInputPointer::SendEventPointerRelease(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	for (u32 i = 0; i < arPointerListeners.Size(); i++)
	{
		ASSERT_NULL(arPointerListeners[i]);
		arPointerListeners[i]->OnInputPointerRelease(ev);
	}
}


} // namespace


