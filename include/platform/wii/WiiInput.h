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

#include "WiiHomeButton.h"

#ifdef USE_AILIVE
#include "WiiAiLive.h"
#endif // USE_AILIVE

namespace Seed { namespace WII {

class Input : public IInput, public IInputPointer, public IInputMotion
{
#ifdef USE_AILIVE
	friend class Seed::WII::AiLive;
#endif // USE_AILIVE
	friend class HomeButton;

	public:
		Input();
		virtual ~Input();

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

	public:
		static Input instance;

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

Input *const pInput = &Input::instance;

}} // namespace

#else // _WII_
	#error "Include 'Input.h' instead 'platform/wii/WiiInputPointer.h' directly."
#endif // _WII_
#endif // __WII_POINTER_INPUT_H__
