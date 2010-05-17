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

/*! \file IphInput.cpp
	\author	Danny Angelo Carminati Grein
	\brief Input Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "IphoneView.h"
#include "EventInputPointer.h"

#define TAG "[Input] "

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(Input);

Input::Input()
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
	Log(TAG "Initialization completed.");

	return r;
}

BOOL Input::Update(f32 delta)
{
	UNUSED(delta);
	for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		memcpy(&oldState[i], &curState[i], sizeof(curState[i]));
		memcpy(&curState[i].touch, &iphTouchBuff[i], sizeof(iphTouchBuff[i]));

		switch (oldState[i].touch.bStatus)
		{
			case 0: // Nada
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 1)
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 2) // nunca deve acontecer
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 1;//curState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = FALSE;
					Log(TAG "Invalid State: 0 -> 2");
				}
				else if (curState[i].touch.bStatus == 3) // nunca deve acontecer
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//curState[i].touch.iTaps;
					curState[i].bValid = FALSE;
					Log(TAG "Invalid State: 0 -> 3");
				}
			}
			break;

			case 1: // Trig
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 1) // clica e segura sem mover
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 2)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 3)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = TRUE;
				}
			}
			break;

			case 2: // Move
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 1) // nunca deve acontecer
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = FALSE;
					Log(TAG "Invalid State: 2 -> 1");
				}
				else if (curState[i].touch.bStatus == 2)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 3)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = TRUE;
				}
			}
			break;

			case 3: // Release
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 1)
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
				else if (curState[i].touch.bStatus == 2) // nunca deve acontecer
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 1;//curState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = FALSE;
					Log(TAG "Invalid State: 3 -> 2");
				}
				else if (curState[i].touch.bStatus == 3) // nada mudou no frame
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = TRUE;
				}
			}
			break;

			default:
			break;
		}

		/*
		if (oldState[i].touch.bStatus != curState[i].touch.bStatus)
		{
			fprintf(stdout, "Old: %d iTrig: %x iHold: %x iRelease: %x iTaps: %d\n", oldState[i].touch.bStatus, oldState[i].iTrig, oldState[i].iHold, oldState[i].iRelease, oldState[i].touch.iTaps);
			fprintf(stdout, "New: %d iTrig: %x iHold: %x iRelease: %x iTaps: %d\n", curState[i].touch.bStatus, curState[i].iTrig, curState[i].iHold, curState[i].iRelease, curState[i].touch.iTaps);

			Log("");
		}

		if (!curState[i].bValid)
			Log(TAG "Invalid state happened.");
		*/
	}

	this->SendEvents();

	return TRUE;
}

INLINE BOOL Input::Reset()
{
	return TRUE;
}

INLINE BOOL Input::IsPressed(u32 button, u16 joystick) const
{
	return (curState[joystick].iTrig && button);
}

INLINE BOOL Input::IsHold(u32 button, u16 joystick) const
{
	return (curState[joystick].iHold && button);
}

BOOL Input::IsReleased(u32 button, u16 joystick) const
{
	return (curState[joystick].iRelease && button);
}

INLINE BOOL Input::IsEnabled(u16 joystick) const
{
	return (joystick == 0);
}

INLINE f32 Input::GetX(u16 joystick) const
{
	f32 w = pScreen->GetWidth();
	f32 x = 0;

	if (pScreen->GetMode() == Screen::LANDSCAPE)
	{
		x = curState[joystick].touch.fPosY;
	}
	else if (pScreen->GetMode() == Screen::LANDSCAPE_GOOFY)
	{
		x = w - curState[joystick].touch.fPosY;
	}
	else
	{
		x = curState[joystick].touch.fPosX;
	}

	return x / w;
}

INLINE f32 Input::GetY(u16 joystick) const
{
	f32 h = pScreen->GetHeight();
	f32 y = 0;

	if (pScreen->GetMode() == Screen::LANDSCAPE)
	{
		y = h - curState[joystick].touch.fPosX;
	}
	else if (pScreen->GetMode() == Screen::LANDSCAPE_GOOFY)
	{
		y = curState[joystick].touch.fPosX;
	}
	else
	{
		y = curState[joystick].touch.fPosY;
	}

	return y / h;
}

INLINE f32 Input::GetRelativeX(u16 joystick) const
{
	f32 w = pScreen->GetWidth();
	f32 nX = 0;
	f32 oX = 0;

	if (pScreen->GetMode() == Screen::LANDSCAPE)
	{
		nX = w - curState[joystick].touch.fPosY;
		oX = w - oldState[joystick].touch.fPosY;
	}
	else if (pScreen->GetMode() == Screen::LANDSCAPE_GOOFY)
	{
		nX = curState[joystick].touch.fPosY;
		oX = oldState[joystick].touch.fPosY;
	}
	else
	{
		nX = curState[joystick].touch.fPosX;
		oX = oldState[joystick].touch.fPosX;
	}

	return (nX - oX) / w;
}

INLINE f32 Input::GetRelativeY(u16 joystick) const
{
	f32 h = pScreen->GetHeight();
	f32 nY = 0;
	f32 oY = 0;

	if (pScreen->GetMode() == Screen::LANDSCAPE)
	{
		nY = h - curState[joystick].touch.fPosX;
		oY = h - oldState[joystick].touch.fPosX;
	}
	else if (pScreen->GetMode() == Screen::LANDSCAPE_GOOFY)
	{
		nY = curState[joystick].touch.fPosX;
		oY = oldState[joystick].touch.fPosX;
	}
	else
	{
		nY = curState[joystick].touch.fPosY;
		oY = oldState[joystick].touch.fPosY;
	}

	return (nY - oY) / h;
}

INLINE f32 Input::GetOrientation(u16 joystick) const
{
	return 1;
}

INLINE f32 Input::GetDistance(u16 joystick) const
{
	return 1;
}

INLINE void Input::SendEvents()
{
	for (u16 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		EventInputPointer ev(i,
							 ConvertButtonFlags(curState[i].iTrig),
							 ConvertButtonFlags(curState[i].iHold),
							 ConvertButtonFlags(curState[i].iRelease),
							 this->GetX(i),
							 this->GetY(i));

		if (curState[i].iTrig)
		{
			this->SendEventPress(&ev);
		}

		if (curState[i].iRelease)
		{
			this->SendEventRelease(&ev);
		}

		if (curState[i].touch.fPosX != oldState[i].touch.fPosX || curState[i].touch.fPosY != oldState[i].touch.fPosY)
		{
			this->SendEventMove(&ev);
		}

		curState[i].iRelease = 0;
		curState[i].iTrig = 0;
	}
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
	return 1;
}

INLINE void Input::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(joystick);
	UNUSED(sens);
}

INLINE Seed::eInputButton Input::GetButtonCode(u32 button) const
{
	Seed::eInputButton btn = Seed::ButtonInvalid;
	switch (button)
	{
		case 1:
			btn = Seed::Button0;
		break;

		case 2:
			btn = Seed::Button1;
		break;
	}

	return btn;
}

INLINE u32 Input::ConvertButtonFlags(u32 flags)
{
	u32 iFlags = 0;

	iFlags = (this->GetButtonCode(flags));

	return iFlags;
}

}} // namespace

#endif // _IPHONE_
