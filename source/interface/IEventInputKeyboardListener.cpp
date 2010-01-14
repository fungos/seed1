/*! \file IEventInputKeyboardListener.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event motion listener class interface
*/

#include "interface/IEventInputKeyboardListener.h"


namespace Seed {


IEventInputKeyboardListener::IEventInputKeyboardListener()
{
}

IEventInputKeyboardListener::~IEventInputKeyboardListener()
{
}

void IEventInputKeyboardListener::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	UNUSED(ev);
}

void IEventInputKeyboardListener::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	UNUSED(ev);
}


} // namespace

