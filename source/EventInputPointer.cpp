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

