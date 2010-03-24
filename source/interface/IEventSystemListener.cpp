/*! \file IEventSystemListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event system listener class interface
*/

#include "interface/IEventSystemListener.h"
#include "Defines.h"

namespace Seed {

IEventSystemListener::IEventSystemListener()
{
}

IEventSystemListener::~IEventSystemListener()
{
}

void IEventSystemListener::OnSystemReset(const EventSystem *ev)
{
	UNUSED(ev);
}

void IEventSystemListener::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev);
}

void IEventSystemListener::OnSystemMenuCalled(const EventSystem *ev)
{
	UNUSED(ev);
}

void IEventSystemListener::OnSystemDataManagerCalled(const EventSystem *ev)
{
	UNUSED(ev);
}

void IEventSystemListener::OnSystemHomeEnded(const EventSystem *ev)
{
	UNUSED(ev);
}

void IEventSystemListener::OnSystemSleep(const EventSystem *ev)
{
	UNUSED(ev);
}

void IEventSystemListener::OnSystemLanguageChanged(const EventSystem *ev)
{
	UNUSED(ev);
}

} // namespace
