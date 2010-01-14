/*! \file IEventInputPointerListener.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event pointer listener class interface
*/

#include "interface/IEventInputPointerListener.h"


namespace Seed {


IEventInputPointerListener::IEventInputPointerListener()
{
}

IEventInputPointerListener::~IEventInputPointerListener()
{
}

void IEventInputPointerListener::OnInputPointerPress(const EventInputPointer *ev)
{
	UNUSED(ev);
}

void IEventInputPointerListener::OnInputPointerRelease(const EventInputPointer *ev)
{
	UNUSED(ev);
}

void IEventInputPointerListener::OnInputPointerMove(const EventInputPointer *ev)
{
	UNUSED(ev);
}

void IEventInputPointerListener::OnInputPointerEnable(const EventInputPointer *ev)
{
	UNUSED(ev);
}

void IEventInputPointerListener::OnInputPointerDisable(const EventInputPointer *ev)
{
	UNUSED(ev);
}

void IEventInputPointerListener::OnAccelerationChanged(const EventInputMotion *ev)
{
	UNUSED(ev);
}


} // namespace

