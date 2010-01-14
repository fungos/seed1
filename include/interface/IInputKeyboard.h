/*! \file IInputKeyboard.h
	\author	Danny Angelo Carminati Grein
	\brief Defines Input Keyboard class interface
*/

#ifndef __IINPUT_KEYBOARD_H__
#define __IINPUT_KEYBOARD_H__

#include "interface/IInput.h"
#include "Array.h"


namespace Seed {


class EventInputKeyboard;
class IEventInputKeyboardListener;


/// Keyboard Input Interface
/**
Platforms that support Keyboard (physical) input must implement this interface.
*/
class IInputKeyboard
{
	public:
		IInputKeyboard();
		virtual ~IInputKeyboard();
/*
		virtual BOOL IsKeyHold(u32 key) const;
		virtual BOOL IsKeyPressed(u32 key) const;
		virtual BOOL IsKeyReleased(u32 key) const;
*/
		virtual void AddKeyboardListener(IEventInputKeyboardListener *listener);
		virtual void RemoveKeyboardListener(IEventInputKeyboardListener *listener);

	protected:
		void SendEventKeyboardPress(const EventInputKeyboard *ev);
		void SendEventKeyboardRelease(const EventInputKeyboard *ev);

	protected:
		Array<IEventInputKeyboardListener *, 1000> arKeyboardListeners;

	private:
		SEED_DISABLE_COPY(IInputKeyboard);
};


} // namespace


#endif // __IINPUT_POINTER_H__

