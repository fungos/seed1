/*! \file IEvent.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event class interface
*/


#ifndef __IEVENT_H__
#define __IEVENT_H__

#include "Defines.h"
#include "IObject.h"


#define EVENT_UNDEFINED			0
#define	EVENT_INPUT_POINTER		1
#define EVENT_INPUT_JOYSTICK	2
#define EVENT_INPUT_MOVEMENT	3
#define EVENT_INPUT_KEYBOARD	4
#define EVENT_SYSTEM			5
#define EVENT_RESOURCE_LOADER	6
#define EVENT_FILE_SYSTEM		7
#define EVENT_MOVIE				8


namespace Seed {


class IEvent : public IObject
{
	public:
		enum eType
		{
			UNDEFINED 	= 0,
			STM			= 1,
			INPUT		= 2,
			SYSTEM		= 3,
			RESOURCE	= 4,
			MOVIE		= 5,
			USER 		= 0x1000
		};

	public:
		IEvent();
		IEvent(eType myType, u32 myId);
		virtual ~IEvent();

		u32 GetId() const;
		eType GetEventType();
		void Consume() const;
		BOOL IsConsumed() const;

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		eType 	iType;
		u32 	iId;
		mutable BOOL	bConsumed;

	private:
		SEED_DISABLE_COPY(IEvent);
};


} // namespace


#endif // __IEVENT_H__
