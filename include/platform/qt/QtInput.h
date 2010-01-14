/*! \file QtInput.h
	\author	Danny Angelo Carminati Grein
	\brief Input Implementation
*/


#ifndef __QT_INPUT_H__
#define __QT_INPUT_H__

#ifdef _QT_

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "interface/IInputKeyboard.h"


namespace Seed { namespace QT {

class Scene;

class Input : public IInput, public IInputPointer, public IInputKeyboard
{
	friend class Scene;

	public:
		Input();
		virtual ~Input();

		// IInput
		virtual eInputButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		virtual BOOL IsPointer() const;
		virtual BOOL IsKeyboard() const;

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

		static Input instance;

	protected:
		void PointerMove(f32 x, f32 y, u16 joystick = 0);
		void PointerPress(f32 x, f32 y, u32 btn, u16 joystick = 0);
		void PointerRelease(f32 x, f32 y, u32 btn, u16 joystick = 0);
		void KeyboardPress(u32 key, u32 modifier, u16 joystick = 0);
		void KeyboardRelease(u32 key, u32 modifier, u16 joystick = 0);

	protected:
		f32 fX;
		f32 fY;

	private:
		SEED_DISABLE_COPY(Input);

		eModifier TranslateModifier(u32 mod);
		eKey TranslateKey(u32 key);
};


Input *const pInput = &Input::instance;


}} // namespace


#else // _QT_

	#error "Include 'Input.h' instead 'platform/qt/QtInput.h' directly."

#endif // _QT_
#endif // __QT_INPUT_H__
