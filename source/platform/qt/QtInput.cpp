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

/*! \file QtInput.cpp
	\author	Danny Angelo Carminati Grein
	\brief Input Implementation
*/

#if defined(_QT_)

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
#include "Enum.h"

#define TAG "[Input] "

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(Input);

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
	IModule::Shutdown();
	Log(TAG "Terminated.");

	return r;
}

INLINE BOOL Input::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	BOOL r = this->Reset();
	Log(TAG "Initialization completed.");

	return r;
}

INLINE BOOL Input::Update(f32 dt)
{
	UNUSED(dt);
	return TRUE;
}

INLINE BOOL Input::Reset()
{
	IModule::Reset();
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

INLINE eInputButton Input::GetButtonCode(u32 button) const
{
	UNUSED(button);
	return Seed::ButtonLeft;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	UNUSED(flags);
	return 0;
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

INLINE BOOL Input::IsPointer() const
{
	return TRUE;
}

INLINE BOOL Input::IsKeyboard() const
{
	return TRUE;
}

INLINE void Input::PointerMove(f32 x, f32 y, u16 joystick)
{
	EventInputPointer ev(joystick, 0, 0, 0, x, y);
	this->SendEventPointerMove(&ev);
}

INLINE void Input::PointerPress(f32 x, f32 y, u32 btn, u16 joystick)
{
	EventInputPointer ev(joystick, btn, 0, 0, x, y);
	this->SendEventPointerPress(&ev);
}

INLINE void Input::PointerRelease(f32 x, f32 y, u32 btn, u16 joystick)
{
	EventInputPointer ev(joystick, 0, 0, btn, x, y);
	this->SendEventPointerRelease(&ev);
}

INLINE void Input::KeyboardPress(u32 key, u32 modifier, u16 joystick)
{
	UNUSED(joystick)
	EventInputKeyboard ev(this->TranslateKey(key), this->TranslateModifier(modifier), key);
	this->SendEventKeyboardPress(&ev);
}

INLINE void Input::KeyboardRelease(u32 key, u32 modifier, u16 joystick)
{
	UNUSED(joystick)
	EventInputKeyboard ev(this->TranslateKey(key), this->TranslateModifier(modifier), key);
	this->SendEventKeyboardRelease(&ev);
}

INLINE eModifier Input::TranslateModifier(u32 mod)
{
	eModifier m = Seed::ModifierNone;

	switch (mod)
	{
		case Qt::ShiftModifier:
		{
			m = Seed::ModifierShift;
		}
		break;

		case Qt::ControlModifier:
		{
			m = Seed::ModifierControl;
		}
		break;

		case Qt::AltModifier:
		{
			m = Seed::ModifierAlt;
		}
		break;

		case Qt::MetaModifier:
		{
			m = Seed::ModifierMeta;
		}
		break;

		case Qt::KeypadModifier:
		{
			m = Seed::ModifierKeypad;
		}
		break;

		case Qt::NoModifier:
		default:
		{
		}
		break;
	}

	return m;
}

INLINE eKey Input::TranslateKey(u32 key)
{
	u32 k = key;

	if (k >= 'a' && k <= 'z')
	{
		k = key - 'a' - 'A';
	}
	else if (k >= ' ' && k <= '~')
	{
		k = key;
	}
	else
	{
		eKey t = KeyNone;

		switch (key)
		{
			case Qt::Key_Escape:
				t = KeyEscape;
			break;

			case Qt::Key_Tab:
				t = KeyTab;
			break;

			case Qt::Key_Backspace:
				t = KeyBackspace;
			break;

			case Qt::Key_Return:
				t = KeyReturn;
			break;

			case Qt::Key_Enter:
				t = KeyEnter;
			break;

			case Qt::Key_Pause:
				t = KeyPause;
			break;

			case Qt::Key_Clear:
				t = KeyClear;
			break;

			case Qt::Key_Delete:
				t = KeyDelete;
			break;

			case Qt::Key_Insert:
				t = KeyInsert;
			break;

			case Qt::Key_Print:
				t = KeyPrintScreen;
			break;

			case Qt::Key_SysReq:

			case Qt::Key_Home:
				t = KeyHome;
			break;

			case Qt::Key_End:
				t = KeyEnd;
			break;

			case Qt::Key_Left:
				t = KeyLeft;
			break;

			case Qt::Key_Up:
				t = KeyUp;
			break;

			case Qt::Key_Down:
				t = KeyDown;
			break;

			case Qt::Key_Right:
				t = KeyRight;
			break;

			case Qt::Key_PageUp:
				t = KeyPageUp;
			break;

			case Qt::Key_PageDown:
				t = KeyPageDown;
			break;

			case Qt::Key_Shift:
				t = KeyShiftLeft;
			break;

			case Qt::Key_Control:
				t = KeyControlLeft;
			break;

			case Qt::Key_Meta:
				t = KeyMetaLeft;
			break;

			case Qt::Key_Alt:
				t = KeyAltLeft;
			break;

			case Qt::Key_Super_L:
				t = KeySuperLeft;
			break;

			case Qt::Key_Super_R:
				t = KeySuperRight;
			break;

			case Qt::Key_AltGr:
				t = KeyAltGr;
			break;

			case Qt::Key_CapsLock:
				t = KeyCapsLock;
			break;

			case Qt::Key_NumLock:
				t = KeyNumLock;
			break;

			case Qt::Key_ScrollLock:
				t = KeyScrollLock;
			break;

			case Qt::Key_F1:
				t = KeyF1;
			break;

			case Qt::Key_F2:
				t = KeyF2;
			break;

			case Qt::Key_F3:
				t = KeyF3;
			break;

			case Qt::Key_F4:
				t = KeyF4;
			break;

			case Qt::Key_F5:
				t = KeyF5;
			break;

			case Qt::Key_F6:
				t = KeyF6;
			break;

			case Qt::Key_F7:
				t = KeyF7;
			break;

			case Qt::Key_F8:
				t = KeyF8;
			break;

			case Qt::Key_F9:
				t = KeyF9;
			break;

			case Qt::Key_F10:
				t = KeyF10;
			break;

			case Qt::Key_F11:
				t = KeyF11;
			break;

			case Qt::Key_F12:
				t = KeyF12;
			break;

			case Qt::Key_F13:
				t = KeyF13;
			break;

			case Qt::Key_F14:
				t = KeyF14;
			break;

			case Qt::Key_F15:
				t = KeyF15;
			break;

			case Qt::Key_Menu:
				t = KeyMenu;
			break;

			case Qt::Key_F16:
			case Qt::Key_F17:
			case Qt::Key_F18:
			case Qt::Key_F19:
			case Qt::Key_F20:
			case Qt::Key_F21:
			case Qt::Key_F22:
			case Qt::Key_F23:
			case Qt::Key_F24:
			case Qt::Key_F25:
			case Qt::Key_F26:
			case Qt::Key_F27:
			case Qt::Key_F28:
			case Qt::Key_F29:
			case Qt::Key_F30:
			case Qt::Key_F31:
			case Qt::Key_F32:
			case Qt::Key_F33:
			case Qt::Key_F34:
			case Qt::Key_F35:
			case Qt::Key_Hyper_L:
			case Qt::Key_Hyper_R:
			case Qt::Key_Help:
			case Qt::Key_Direction_L:
			case Qt::Key_Direction_R:
			case Qt::Key_unknown:
			default:
			break;
		}

		k = static_cast<u32>(t);
	}

	return static_cast<eKey>(k);
}

}} // namespace

#endif // _QT_
