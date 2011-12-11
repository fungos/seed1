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

/*! \file PS3Input.cpp
	\author	Danny Angelo Carminati Grein
	\brief Input Implementation
*/

#if defined(_PS3DEV_)

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "Trigonometry.h"
#include "System.h"
#include "Input.h"
#include "EventInputPointer.h"
#include "EventInputJoystick.h"
#include "EventSystem.h"
#include "ViewManager.h"
#include "Viewport.h"

#define TAG "[Input] "

namespace Seed { namespace PS3 {

SEED_SINGLETON_DEFINE(Input)

Input::Input()
	: iJoystickCount(0)
	, fX(0.0f)
	, fY(0.0f)
{
}

Input::~Input()
{
	this->Reset();
}

INLINE BOOL Input::Shutdown()
{
	Log(TAG "Terminating...");

	ioPadEnd();

	BOOL r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

INLINE BOOL Input::Initialize()
{
	Log(TAG "Initializing...");
	BOOL r = this->Reset();

	ioPadInit(MAX_JOYSTICKS);

	ioPadGetInfo(&cPadInfo);
	ioPadGetInfo2(&cPadInfo2);

	padCapabilityInfo info;

	iJoystickCount = cPadInfo2.max;
	if (iJoystickCount)
	{
		Log(TAG "Joystick(s): ");
		for (u32 i = 0; i < iJoystickCount; i++)
		{
			s32 ret = ioPadGetCapabilityInfo(i, &info);
			if (ret)
			{
				Log("Opened Joystick %d:", i);
				Log(TAG "\tProduct Id..: %d", cPadInfo.product_id[i]);
				Log(TAG "\tVendor Id...: %d", cPadInfo.vendor_id[i]);
				Log(TAG "\tPS3Spec.....: %d", info.ps3spec);
				Log(TAG "\tHas Pressure: %d", info.has_pressure);
				Log(TAG "\tHas Sensors.: %d", info.has_sensors);
				Log(TAG "\tHas HPS.....: %d", info.has_hps);
				Log(TAG "\tHas Vibrate.: %d", info.has_vibrate);
			}
		}
	}
	Log(TAG "Initialization completed.");

	return r;
}

INLINE BOOL Input::Update(f32 dt)
{
	UNUSED(dt);

	ioPadGetInfo2(&cPadInfo2);
	iJoystickCount = cPadInfo2.max;

	for (u32 i = 0; i < iJoystickCount; i++)
	{
		padData *e = &cData[i];
		ioPadGetData(i, e);

		if (e->BTN_DOWN)
		{
//			EventInputKeyboard ev(event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.scancode, 0);
//			this->SendEventKeyboardPress(&ev);
		}
		else if (e->BTN_UP)
		{
//			EventInputKeyboard ev(event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.scancode, 0);
//			this->SendEventKeyboardRelease(&ev);
		}
//		else if (e->QUIT)
//		{
//			EventSystem ev;
//			pSystem->SendEventShutdown(&ev);
//		}
/*
			case SDL_MOUSEMOTION:
			{
				f32 x, y;
				x = fX = (f32)event.motion.x / (f32)pScreen->GetWidth();
				y = fY = (f32)event.motion.y / (f32)pScreen->GetHeight();

				Viewport *viewport = static_cast<Viewport*>(pViewManager->GetViewportAt(fX, fY));
				f32 fw = 1.0f;
				f32 fh = 1.0f;
				if (viewport)
				{
					fw = (viewport->GetWidth());// * viewport->GetWidth());
					fh = (viewport->GetHeight());// * viewport->GetHeight());
					//x = viewport->GetX() + viewport->GetWidth() * fX;
					//y = viewport->GetY() + viewport->GetHeight() * fY;
					x = (fX - viewport->GetX()) / fw;
					y = (fY - viewport->GetY()) / fh;
				}

				EventInputPointer ev(0, 0, 0, 0, x, y);
				this->SendEventPointerMove(&ev);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				f32 x, y;
				x = fX = (f32)event.motion.x / (f32)pScreen->GetWidth();
				y = fY = (f32)event.motion.y / (f32)pScreen->GetHeight();

				Viewport *viewport = static_cast<Viewport*>(pViewManager->GetViewportAt(fX, fY));
				f32 fw = 1.0f;
				f32 fh = 1.0f;
				if (viewport)
				{
					fw = (viewport->GetWidth());// * viewport->GetWidth());
					fh = (viewport->GetHeight());// * viewport->GetHeight());
					//x = viewport->GetX() + viewport->GetWidth() * fX;
					//y = viewport->GetY() + viewport->GetHeight() * fY;
					x = (fX - viewport->GetX()) / fw;
					y = (fY - viewport->GetY()) / fh;
				}

				const EventInputPointer ev(0, 0, 0, this->ConvertButtonFlags(event.button.button), x, y);
				this->SendEventPointerRelease(&ev);
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				f32 x, y;
				x = fX = (f32)event.motion.x / (f32)pScreen->GetWidth();
				y = fY = (f32)event.motion.y / (f32)pScreen->GetHeight();

				Viewport *viewport = static_cast<Viewport*>(pViewManager->GetViewportAt(fX, fY));
				f32 fw = 1.0f;
				f32 fh = 1.0f;
				if (viewport)
				{
					fw = (viewport->GetWidth());// * viewport->GetWidth());
					fh = (viewport->GetHeight());// * viewport->GetHeight());
					//x = viewport->GetX() + viewport->GetWidth() * fX;
					//y = viewport->GetY() + viewport->GetHeight() * fY;
					x = (fX - viewport->GetX()) / fw;
					y = (fY - viewport->GetY()) / fh;
				}

				const EventInputPointer ev(0, this->ConvertButtonFlags(event.button.button), 0, 0, x, y);
				this->SendEventPointerPress(&ev);
			}
			break;

			case SDL_JOYBUTTONDOWN:
			{
				const EventInputJoystick ev(event.jbutton.which, event.jbutton.button, 0, 0, 0, 0);
				this->SendEventJoystickButtonPress(&ev);
			}
			break;

			case SDL_JOYBUTTONUP:
			{
				const EventInputJoystick ev(event.jbutton.which, 0, 0, event.jbutton.button, 0, 0);
				this->SendEventJoystickButtonRelease(&ev);
			}
			break;

			case SDL_JOYAXISMOTION:
			{
				const EventInputJoystick ev(event.jbutton.which, 0, 0, 0, event.jaxis.axis, event.jaxis.value);
				this->SendEventJoystickAxisMove(&ev);
			}
			break;

			case SDL_JOYHATMOTION:
			{
				const EventInputJoystick ev(event.jbutton.which, 0, 0, 0, event.jhat.hat, event.jhat.value);
				this->SendEventJoystickDPadMove(&ev);
			}
			break;

			case SDL_JOYBALLMOTION:
			break;

			default:
			break;
		}
		*/
	}

	return TRUE;
}

INLINE BOOL Input::Reset()
{
	return TRUE;
}

INLINE BOOL Input::IsPressed(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL Input::IsHold(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL Input::IsReleased(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return FALSE;
}

INLINE BOOL Input::IsEnabled(u16 joystick) const
{
	UNUSED(joystick);
	return (joystick == 0);
}

INLINE f32 Input::GetX(u16 joystick) const
{
	UNUSED(joystick);
	return this->fX;
}

INLINE f32 Input::GetY(u16 joystick) const
{
	UNUSED(joystick);
	return this->fY;
}

INLINE f32 Input::GetRelativeX(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE f32 Input::GetRelativeY(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE f32 Input::GetOrientation(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE f32 Input::GetDistance(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE Seed::eInputButton Input::GetButtonCode(u32 button) const
{
	Seed::eInputButton btn = Seed::ButtonInvalid;

	if (button & SDL_BUTTON_LMASK)
		btn = Seed::ButtonLeft;
	else if (button & SDL_BUTTON_RMASK)
		btn = Seed::ButtonRight;
	else if (button & SDL_BUTTON_MMASK)
		btn = Seed::ButtonMiddle;
	else if (button & SDL_BUTTON(SDL_BUTTON_WHEELUP))
		btn = Seed::ButtonUp;
	else if (button & SDL_BUTTON(SDL_BUTTON_WHEELDOWN))
		btn = Seed::ButtonDown;

	return btn;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	u32 converted = 0;

	converted |= this->GetButtonCode(SDL_BUTTON(flags));

	return converted;
}

INLINE u32 Input::GetSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE u32 Input::GetMaximumSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

INLINE u32 Input::GetMaximumPointers() const
{
	return 0;
}

INLINE void Input::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(sens);
	UNUSED(joystick);
}

INLINE Seed::eKey Input::GetKeyCode(u32 key) const
{
	Seed::eKey k = static_cast<Seed::eKey>(key);

	return k;
}

INLINE Seed::eModifier Input::GetModifierCode(u32 mod) const
{
	Seed::eModifier m = static_cast<Seed::eModifier>(mod);

	return m;
}

INLINE u32 Input::GetMaximumJoysticks() const
{
	return iJoystickCount;
}

INLINE BOOL Input::IsPointer() const
{
	return TRUE;
}

//INLINE BOOL Input::IsMotion() const;
INLINE BOOL Input::IsJoystick() const
{
	return iJoystickCount > 0;
}

INLINE BOOL Input::IsKeyboard() const
{
	return TRUE;
}

}} // namespace

#endif // _PS3DEV_
