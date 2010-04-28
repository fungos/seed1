/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

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

class SEED_CORE_API IEvent : public IObject
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
