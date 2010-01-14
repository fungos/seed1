/*! \file EventInputKeyboard.cpp
	\author	Danny Angelo Carminati Grein
	\brief A event from an input keyboard
*/

#include "EventInputKeyboard.h"
#include "Enum.h"


namespace Seed {


EventInputKeyboard::EventInputKeyboard(u32 key, u32 modifier, u32 scan, u32 keyboard)
	: IEventInput(EVENT_INPUT_KEYBOARD)
	, cKey(key)
	, iModifier(modifier)
	, iScancode(scan)
	, iKeyboard(keyboard)
{
}

EventInputKeyboard::~EventInputKeyboard()
{
}

INLINE const Key &EventInputKeyboard::GetKey() const
{
	return cKey;
}

INLINE u32 EventInputKeyboard::GetModifier() const
{
	return iModifier;
}

INLINE u32 EventInputKeyboard::GetScancode() const
{
	return iScancode;
}

INLINE u32 EventInputKeyboard::GetKeyboard() const
{
	return iKeyboard;
}

INLINE BOOL EventInputKeyboard::IsShiftPressed() const
{
	return (iModifier & ModifierShift) > 0;
}

INLINE BOOL EventInputKeyboard::IsShiftLeftPressed() const
{
	return (iModifier & Seed::ModifierShiftLeft) > 0;
}

INLINE BOOL EventInputKeyboard::IsShiftRightPressed() const
{
	return (iModifier & Seed::ModifierShiftRight) > 0;
}

INLINE BOOL EventInputKeyboard::IsAltPressed() const
{
	return (iModifier & Seed::ModifierAlt) > 0;
}

INLINE BOOL EventInputKeyboard::IsAltLeftPressed() const
{
	return (iModifier & Seed::KeyAltLeft) > 0;
}

INLINE BOOL EventInputKeyboard::IsAltRightPressed() const
{
	return (iModifier & Seed::ModifierAltRight) > 0;
}

INLINE BOOL EventInputKeyboard::IsControlPressed() const
{
	return (iModifier & Seed::ModifierControl) > 0;
}

INLINE BOOL EventInputKeyboard::IsControlLeftPressed() const
{
	return (iModifier & Seed::ModifierControlLeft) > 0;
}

INLINE BOOL EventInputKeyboard::IsControlRightPressed() const
{
	return (iModifier & Seed::ModifierControlRight) > 0;
}

INLINE BOOL EventInputKeyboard::IsMetaPressed() const
{
	return (iModifier & Seed::ModifierMeta) > 0;
}

INLINE BOOL EventInputKeyboard::IsMetaLeftPressed() const
{
	return (iModifier & Seed::ModifierMetaLeft) > 0;
}

INLINE BOOL EventInputKeyboard::IsMetaRightPressed() const
{
	return (iModifier & Seed::ModifierMetaRight) > 0;
}

INLINE BOOL EventInputKeyboard::IsCapsLockOn() const
{
	return (iModifier & Seed::ModifierCapsLock) > 0;
}

INLINE BOOL EventInputKeyboard::IsNumLockOn() const
{
	return (iModifier & Seed::ModifierNumLock) > 0;
}

INLINE BOOL EventInputKeyboard::IsNumericPad() const
{
	return (cKey.GetValue() >= Seed::KeyPad0 && cKey.GetValue() <= Seed::KeyPadEquals);
}

INLINE BOOL EventInputKeyboard::IsCharacter() const
{
	return cKey.IsCharacter();
}

INLINE BOOL EventInputKeyboard::IsNumber() const
{
	return cKey.IsNumber();
}

INLINE BOOL EventInputKeyboard::IsLetter() const
{
	return cKey.IsLetter();
}

INLINE const char *EventInputKeyboard::GetObjectName() const
{
	return "EventInputKeyboard";
}


} // namespace

