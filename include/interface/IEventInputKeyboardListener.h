/*! \file IEventInputKeyboardListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event keyboard listener class interface
*/


#ifndef __IEVENT_INPUT_KEYBOARD_LISTENER_H__
#define __IEVENT_INPUT_KEYBOARD_LISTENER_H__


#include "interface/IEventListener.h"
#include "EventInputKeyboard.h"


namespace Seed {


class IEventInputKeyboardListener : public IEventListener
{
	public:
		IEventInputKeyboardListener();
		virtual ~IEventInputKeyboardListener();

		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	private:
		SEED_DISABLE_COPY(IEventInputKeyboardListener);
};


} // namespace


#endif // __IEVENT_INPUT_KEYBOARD_LISTENER_H__

