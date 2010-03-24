/*! \file IphInput.h
	\author	Danny Angelo Carminati Grein
	\brief Input Iphone Implementation
*/

#ifndef __IPH_INPUT_H__
#define __IPH_INPUT_H__

#ifdef _IPHONE_

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "IphoneSystemStub.h"

namespace Seed { namespace iPhone {

class Input : public IInput, public IInputPointer
{
	public:
		Input();
		virtual ~Input();

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
		static Input instance;

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

Input *const pInput = &Input::instance;

}} // namespace

#else // _IPHONE_
	#error "Include 'Input.h' instead 'platform/iphone/IphInput.h' directly."
#endif // _IPHONE_
#endif // __IPH_INPUT_H__
