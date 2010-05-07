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

/*! \file IphInput.h
	\author	Danny Angelo Carminati Grein
	\brief Input Iphone Implementation
*/

#ifndef __IPH_INPUT_H__
#define __IPH_INPUT_H__

#if defined(_IPHONE_)

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "IphoneSystemStub.h"
#include "Singleton.h"

namespace Seed { namespace iPhone {

class Input : public IInput, public IInputPointer
{
	SEED_SINGLETON_DECLARE(Input);
	public:
		// IInput
		virtual Seed::eInputButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		// IInputPointer
		virtual BOOL IsEnabled(u16 joystick = 0) const;
		virtual BOOL IsHold(u32 button, u16 joystick = 0) const;
		virtual BOOL IsPressed(u32 button, u16 joystick = 0) const;
		virtual BOOL IsReleased(u32 button, u16 joystick = 0) const;

		virtual f32 GetX(u16 joystick = 0) const;
		virtual f32 GetY(u16 joystick = 0) const;
		virtual f32 GetRelativeX(u16 joystick = 0) const;
		virtual f32 GetRelativeY(u16 joystick = 0) const;
		virtual f32 GetOrientation(u16 joystick = 0) const;
		virtual f32 GetDistance(u16 joystick = 0) const;

		virtual u32 GetSensitivity(u16 joystick = 0) const;
		virtual u32 GetMaximumSensitivity(u16 joystick = 0) const;
		virtual u32 GetMaximumPointers() const;
		virtual void SetSensitivity(u32 sens, u16 joystick = 0);

		// IUpdatable
		virtual BOOL Update(f32 delta);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

	public:
		struct sState
		{
			iphTouchInfo touch;
			u32 iTrig;
			u32 iHold;
			u32 iRelease;
			BOOL bValid;
		};

	private:
		SEED_DISABLE_COPY(Input);

	private:
		void SendEvents();
		sState curState[PLATFORM_MAX_INPUT];
		sState oldState[PLATFORM_MAX_INPUT];
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Input);
}

}} // namespace

#else // _IPHONE_
	#error "Include 'Input.h' instead 'platform/iphone/IphInput.h' directly."
#endif // _IPHONE_
#endif // __IPH_INPUT_H__
