/*! \file EventInputKeyboard.h
	\author	Danny Angelo Carminati Grein
	\brief A event from an input keyboard
*/

#ifndef __EVENT_INPUT_KEYBOARD_H__
#define __EVENT_INPUT_KEYBOARD_H__


#include "interface/IEventInput.h"
#include "Key.h"


namespace Seed {


class EventInputKeyboard : public IEventInput
{
	public:
		EventInputKeyboard(u32 key, u32 modifier, u32 scan, u32 keyboard = 0);
		virtual ~EventInputKeyboard();

		u32 GetScancode() const;
		const Key &GetKey() const;
		u32 GetModifier() const;
		u32	GetKeyboard() const;

		// Modifiers
		BOOL IsShiftPressed() const;
		BOOL IsShiftLeftPressed() const;
		BOOL IsShiftRightPressed() const;

		BOOL IsAltPressed() const;
		BOOL IsAltLeftPressed() const;
		BOOL IsAltRightPressed() const;

		BOOL IsControlPressed() const;
		BOOL IsControlLeftPressed() const;
		BOOL IsControlRightPressed() const;

		BOOL IsMetaPressed() const;
		BOOL IsMetaLeftPressed() const;
		BOOL IsMetaRightPressed() const;

		BOOL IsCapsLockOn() const;
		BOOL IsNumLockOn() const;

		BOOL IsNumericPad() const;

		// Utils
		//! Checks if this key is a character.
		/*!
			\return True if the key is a letter, a number or a whitespace, everything else returns false.
		 */
		BOOL IsCharacter() const;

		//! Checks if this key is a number.
		/*!
			\return True if the key is a a number from 0 to 9, everything else returns false.
		 */
		BOOL IsNumber() const;
		//! Checks if this key is a letter.
		/*!
			\return True if the key is a letter between a-z or A-Z, everything else returns false.
		 */
		BOOL IsLetter() const;

		// IObject
		const char *GetObjectName() const;

	protected:
		//u32 iKey;
		Key	cKey;
		u32 iModifier;
		u32 iScancode;
		u32 iKeyboard;

	private:
		SEED_DISABLE_COPY(EventInputKeyboard);
};


} // namespace


#endif // __EVENT_INPUT_KEYBOARD_H__

