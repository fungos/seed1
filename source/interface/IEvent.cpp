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
