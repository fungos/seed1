/*! \file SdlInput.h
	\author	Danny Angelo Carminati Grein
	\brief Input Implementation
*/


#ifndef __SDL_INPUT_H__
#define __SDL_INPUT_H__

#ifdef _SDL_

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "interface/IInputKeyboard.h"
#include "Enum.h"


namespace Seed { namespace SDL {


class Input : public IInput, public IInputPointer, public IInputKeyboard
{
	public:
		Input();
		virtual ~Input();

		// IInput
		virtual Seed::eInputButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		virtual BOOL IsPointer() const;
		virtual BOOL IsKeyboard() const;

		// IInputKeyboard

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
		virtual BOOL Update(f32 dt);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

	public:
		static Input instance;

	private:
		SEED_DISABLE_COPY(Input);

		Seed::eKey GetKeyCode(u32 key) const;
		Seed::eModifier GetModifierCode(u32 mod) const;

	private:
		f32 fX;
		f32 fY;
};


Input *const pInput = &Input::instance;


}} // namespace


#else // _SDL_

	#error "Include 'Input.h' instead 'platform/sdl/SdlInput.h' directly."

#endif // _SDL_
#endif // __SDL_INPUT_H__
