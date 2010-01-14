/*! \file IEventInput.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event input class interface
*/

#ifndef __IEVENTINPUT_H__
#define __IEVENTINPUT_H__


#include "IEvent.h"


namespace Seed {


class IEventInput : public IEvent
{
	public:
		IEventInput();
		IEventInput(u32 myId);
		virtual ~IEventInput();
};


} // namespace


#endif // __IEVENTINPUT_H__
