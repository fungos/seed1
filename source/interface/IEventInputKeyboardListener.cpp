/*! \file IEventInputKeyboardListener.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event motion listener class interface
*/

#include "interface/IEventInputKeyboardListener.h"
#include "Log.h"

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
	SEED_ABSTRACT_METHOD;
}

void IEventInputKeyboardListener::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

} // namespace
