/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

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
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE BOOL IInputPointer::IsPressed(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE BOOL IInputPointer::IsReleased(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE f32 IInputPointer::GetX(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE f32 IInputPointer::GetY(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE u32 IInputPointer::GetSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE void IInputPointer::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(sens);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
}

INLINE u32 IInputPointer::GetMaximumSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE u32 IInputPointer::GetMaximumPointers() const
{
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE f32 IInputPointer::GetRelativeX(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE f32 IInputPointer::GetRelativeY(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE f32 IInputPointer::GetDistance(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE f32 IInputPointer::GetOrientation(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE BOOL IInputPointer::IsEnabled(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
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
