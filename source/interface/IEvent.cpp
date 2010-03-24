/*! \file IEvent.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event class interface
*/


#include "Defines.h"
#include "interface/IEvent.h"
#include "Enum.h"

namespace Seed {

IEvent::IEvent(eType myType, u32 myId)
	: iType(myType)
	, iId(myId)
	, bConsumed(FALSE)
{
}

IEvent::IEvent()
	: iType(UNDEFINED)
	, iId(EVENT_UNDEFINED)
	, bConsumed(FALSE)
{
}

IEvent::~IEvent()
{
	this->iType 	= UNDEFINED;
	this->iId 		= EVENT_UNDEFINED;
	this->bConsumed = FALSE;
}

INLINE u32 IEvent::GetId() const
{
	return iId;
}

INLINE IEvent::eType IEvent::GetEventType()
{
	return iType;
}

INLINE void IEvent::Consume() const
{
	this->bConsumed = TRUE;
}

INLINE BOOL IEvent::IsConsumed() const
{
	return bConsumed;
}

INLINE int IEvent::GetObjectType() const
{
	return Seed::ObjectEvent;
}

INLINE const char *IEvent::GetObjectName() const
{
	return "IEvent";
}

} // namespace
