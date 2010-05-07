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

/*! \file WiiPointerInput.h
	\author	Danny Angelo Carminati Grein
	\brief Wii Input
*/

#ifndef __WII_INPUT_H__
#define __WII_INPUT_H__

#include "Defines.h"

#if defined(_WII_)

#include "interface/IInputPointer.h"
#include "interface/IInputMotion.h"
#include "Vector.h"
#include "Singleton.h"

#include "WiiHomeButton.h"

#if defined(USE_AILIVE)
#include "WiiAiLive.h"
#endif // USE_AILIVE

namespace Seed { namespace WII {

class Input : public IInput, public IInputPointer, public IInputMotion
{
#if defined(USE_AILIVE)
	friend class Seed::WII::AiLive;
#endif // USE_AILIVE
	friend class HomeButton;

	SEED_SINGLETON_DECLARE(Input);
	public:
		// IInputPointer
		virtual BOOL IsHold(u32 button, u16 joystick = 0) const;
		virtual BOOL IsPressed(u32 button, u16 joystick = 0) const;
		virtual BOOL IsReleased(u32 button, u16 joystick = 0) const;
		virtual BOOL IsEnabled(u16 joystick = 0) const;

		virtual f32 GetX(u16 joystick = 0) const;
		virtual f32 GetY(u16 joystick = 0) const;

		virtual u32 GetSensitivity(u16 joystick = 0) const;
		virtual void SetSensitivity(u32 sens, u16 joystick = 0);
		virtual u32 GetMaximumSensitivity(u16 joystick = 0) const;
		virtual u32 GetMaximumPointers() const;

		virtual f32 GetRelativeX(u16 joystick = 0) const;
		virtual f32 GetRelativeY(u16 joystick = 0) const;
		virtual f32 GetDistance(u16 joystick = 0) const;
		virtual f32 GetOrientation(u16 joystick = 0) const;

		virtual eButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		// IInputMotion
		virtual Vector<f32> GetAccelerationAxis(u16 joystick = 0) const;
		virtual f32 GetAccelerationMagnitude(u16 joystick = 0) const;
		virtual f32 GetAccelerationSpeed(u16 joystick = 0) const;

		// IUpdatable
		virtual BOOL Update(f32 delta);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

	protected:
		WiiInputStatus pad[PLATFORM_MAX_INPUT][WII_MAX_READ_BUFS];
		int padReads[PLATFORM_MAX_INPUT];
		BOOL padEnabled[PLATFORM_MAX_INPUT];

		WiiInputStatus oldPad[PLATFORM_MAX_INPUT][WII_MAX_READ_BUFS];
		int oldPadReads[PLATFORM_MAX_INPUT];
		BOOL oldPadEnabled[PLATFORM_MAX_INPUT];

	private:
		SEED_DISABLE_COPY(Input);
		void SendEvents();
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Input);
}

}} // namespace

#else // _WII_
	#error "Include 'Input.h' instead 'platform/wii/WiiInputPointer.h' directly."
#endif // _WII_
#endif // __WII_POINTER_INPUT_H__
