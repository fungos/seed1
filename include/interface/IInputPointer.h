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

/*! \file IInputPointer.h
	\author	Danny Angelo Carminati Grein
	\brief Defines Input Pointer class interface
*/

#ifndef __IINPUT_POINTER_H__
#define __IINPUT_POINTER_H__

#include "interface/IInput.h"
#include "Array.h"

namespace Seed {

class EventInputPointer;
class IEventInputPointerListener;

/// Pointer Input Interface
/**
Platforms that support Pointer (mouse like) input must implement this interface.
*/
class SEED_CORE_API IInputPointer
{
	public:
		IInputPointer();
		virtual ~IInputPointer();

		virtual BOOL IsHold(u32 button, u16 joystick = 0) const;
		virtual BOOL IsPressed(u32 button, u16 joystick = 0) const;
		virtual BOOL IsReleased(u32 button, u16 joystick = 0) const;
		virtual BOOL IsEnabled(u16 joystick = 0) const;

		// GetX and GetY is based on screen coordinates. Do not use when using Viewports.
		virtual f32 GetX(u16 joystick = 0) const = 0;
		virtual f32 GetY(u16 joystick = 0) const = 0;

		virtual u32 GetSensitivity(u16 joystick = 0) const = 0;
		virtual void SetSensitivity(u32 sens, u16 joystick = 0) = 0;
		virtual u32 GetMaximumSensitivity(u16 joystick = 0) const = 0;
		virtual u32 GetMaximumPointers() const = 0;

		virtual f32 GetRelativeX(u16 joystick = 0) const = 0;
		virtual f32 GetRelativeY(u16 joystick = 0) const = 0;
		virtual f32 GetDistance(u16 joystick = 0) const = 0;
		virtual f32 GetOrientation(u16 joystick = 0) const = 0;

		void AddPointerListener(IEventInputPointerListener *listener);
		void RemovePointerListener(IEventInputPointerListener *listener);

	protected:
		void SendEventPointerEnable(const EventInputPointer *ev);
		void SendEventPointerDisable(const EventInputPointer *ev);
		void SendEventPointerMove(const EventInputPointer *ev);
		void SendEventPointerPress(const EventInputPointer *ev);
		void SendEventPointerRelease(const EventInputPointer *ev);

	protected:
		Array<IEventInputPointerListener *, 1000> arPointerListeners;

	private:
		SEED_DISABLE_COPY(IInputPointer);
};

} // namespace

#endif // __IINPUT_POINTER_H__
