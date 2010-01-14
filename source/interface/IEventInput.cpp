/*! \file IEventInput.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event input class interface
*/


#include "interface/IEventInput.h"


namespace Seed {


IEventInput::IEventInput()
	: IEvent(IEvent::INPUT, 0)
{
}

IEventInput::IEventInput(u32 myId)
	: IEvent(IEvent::INPUT, myId)
{
}

IEventInput::~IEventInput()
{
}


} // namespace
