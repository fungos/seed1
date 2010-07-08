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

/*! \file EventWidget.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines a widget event class
*/

#include "Enum.h"
#include "gui/EventWidget.h"

namespace Seed {

EventWidget::EventWidget(const IWidget *sender, const IWidget *receiver, eWidgetEventType t, u32 p, f32 x, f32 y, u32 pressed, u32 hold, u32 released)
	: pSender(sender)
	, pReceiver(receiver)
	, iType(t)
	, iPlayer(p)
	, fX(x)
	, fY(y)
	, iPressed(pressed)
	, iHold(hold)
	, iReleased(released)
{
}

EventWidget::~EventWidget()
{
	pSender = NULL;
	pReceiver = NULL;
	iType = WidgetEventNone;
}

INLINE const IWidget *EventWidget::GetSender() const
{
	return pSender;
}

INLINE const IWidget *EventWidget::GetReceiver() const
{
	return pReceiver;
}

INLINE eWidgetEventType EventWidget::GetEventType() const
{
	return iType;
}

INLINE f32 EventWidget::GetX() const
{
	return fX;
}

INLINE f32 EventWidget::GetY() const
{
	return fY;
}

INLINE u32 EventWidget::GetPlayer() const
{
	return iPlayer;
}

INLINE u32 EventWidget::GetPressed() const
{
	return iPressed;
}

INLINE u32 EventWidget::GetReleased() const
{
	return iReleased;
}

INLINE u32 EventWidget::GetHold() const
{
	return iHold;
}

} // namespace
