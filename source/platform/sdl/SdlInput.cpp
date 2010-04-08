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

/*! \file SdlInput.cpp
	\author	Danny Angelo Carminati Grein
	\brief Input Implementation
*/


#ifdef _SDL_

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "Trigonometry.h"
#include "System.h"
#include "Input.h"
#include "EventInputPointer.h"
#include "EventInputKeyboard.h"
#include "EventSystem.h"

#include <SDL/SDL_syswm.h>

#define TAG "[Input] "


namespace Seed { namespace SDL {


Input Input::instance;


Input::Input()
	: fX(0.0f)
	, fY(0.0f)
{
}

Input::~Input()
{
	Reset();
}

INLINE BOOL Input::Shutdown()
{
	Log(TAG "Terminating...");
	BOOL r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

INLINE BOOL Input::Initialize()
{
	Log(TAG "Initializing...");
	BOOL r = this->Reset();
	#if defined(WIN32) && defined(_DEBUG_)
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
	#endif
	Log(TAG "Initialization completed.");

	return r;
}

INLINE BOOL Input::Update(f32 dt)
{
	UNUSED(dt);

/*
FIXME: 2009-02-17 | BUG | Usar polling? Isso deve ferrar com o frame rate configurado pelo usuario. Verificar tambem... | Danny Angelo Carminati Grein
*/
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			#if defined(WIN32) && defined(_DEBUG_)
			case SDL_SYSWMEVENT:
			{
				switch (event.syswm.msg->msg)
				{
					case WM_SYSCOMMAND:
					case WM_IME_SETCONTEXT:
					case WM_IME_NOTIFY:
					break;

					case WM_GETTEXT:
					case WM_GETICON:
					case WM_NCHITTEST:
					case WM_NCMOUSEMOVE:
					case WM_NCLBUTTONDOWN:
					case WM_NCLBUTTONDBLCLK:
					case WM_NCLBUTTONUP:
					case WM_CAPTURECHANGED:
					case WM_WINDOWPOSCHANGING:
					case WM_WINDOWPOSCHANGED:
					case WM_MOVE:
					case WM_MOVING:
					case WM_ENTERSIZEMOVE:
					case WM_EXITSIZEMOVE:
					case WM_MOUSEACTIVATE:
					case WM_NCCALCSIZE:
					case WM_SIZE:
					case WM_QUERYOPEN:
					break;

					case WM_DISPLAYCHANGE:
						Log(TAg "event DISPLAYCHANGE");
					break;

					case WM_SYNCPAINT:
						Log(TAG "event SYNCPAINT");
					break;

					case WM_NCPAINT:
						Log(TAG "event NCPAINT");
					break;

					case WM_NCACTIVATE:
						Log(TAG "event NCACTIVATE");
					break;

					case WM_KILLFOCUS:
						Log(TAG "event KILLFOCUS");
					break;

					case WM_SETFOCUS:
						Log(TAG "event SETFOCUS");
					break;

					case WM_ACTIVATEAPP:
						Log(TAG "event ACTIVATEAPP");
					break;

					case WM_TASKBAR_CREATED:
						Log(TAG "event TASKBAR_CREATED");
					break;

					default:
						Log(TAG "Received system event. Message (0x%x) wParam = %d, lParam = %d.", event.syswm.msg->msg, event.syswm.msg->wParam, event.syswm.msg->lParam);
					break;
				}
			}
			break;
			#endif

			case SDL_KEYDOWN:
			{
				EventInputKeyboard ev(event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.scancode, event.key.which);
				this->SendEventKeyboardPress(&ev);
			}
			break;

			case SDL_KEYUP:
			{
				EventInputKeyboard ev(event.key.keysym.sym, event.key.keysym.mod, event.key.keysym.scancode, event.key.which);
				this->SendEventKeyboardRelease(&ev);
			}
			break;

			case SDL_QUIT:
			{
				EventSystem ev;
				pSystem->SendEventShutdown(&ev);
			}
			break;

			case SDL_MOUSEMOTION:
			{
				#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
					this->fX = (f32)event.motion.x;
					this->fY = (f32)event.motion.y;
				#else
					this->fX = (f32)event.motion.x / (f32)pScreen->GetWidth();
					this->fY = (f32)event.motion.y / (f32)pScreen->GetHeight();
				#endif

				EventInputPointer ev(0, 0, 0, 0, this->fX, this->fY);
				this->SendEventPointerMove(&ev);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
					this->fX = (f32)event.motion.x;
					this->fY = (f32)event.motion.y;
				#else
					this->fX = (f32)event.motion.x / (f32)pScreen->GetWidth();
					this->fY = (f32)event.motion.y / (f32)pScreen->GetHeight();
				#endif

				const EventInputPointer ev(0, 0, 0, this->ConvertButtonFlags(event.button.button), fX, fY);
				this->SendEventPointerRelease(&ev);
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
					this->fX = (f32)event.motion.x;
					this->fY = (f32)event.motion.y;
				#else
					this->fX = (f32)event.motion.x / (f32)pScreen->GetWidth();
					this->fY = (f32)event.motion.y / (f32)pScreen->GetHeight();
				#endif

				const EventInputPointer ev(0, this->ConvertButtonFlags(event.button.button), 0, 0, fX, fY);
				this->SendEventPointerPress(&ev);
			}
			break;

			default:
			break;
		}
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

INLINE BOOL Input::IsPointer() const
{
	return TRUE;
}

//INLINE BOOL Input::IsMotion() const;
//INLINE BOOL Input::IsJoystick() const;
INLINE BOOL Input::IsKeyboard() const
{
	return TRUE;
}


}} // namespace


#endif // _SDL_
