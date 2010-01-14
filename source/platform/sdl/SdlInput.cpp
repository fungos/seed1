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
				pSystem->Shutdown();
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
