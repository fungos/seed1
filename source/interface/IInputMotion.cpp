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
	SEED_ABSTRACT_METHOD;
	return Vector3f(0.0f, 0.0f, 0.0f);
}

INLINE f32 IInputMotion::GetAccelerationMagnitude(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

INLINE f32 IInputMotion::GetAccelerationSpeed(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
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
