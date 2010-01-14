/*! \file EventWidget.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines a widget event class
*/


#include "gui/EventWidget.h"


namespace Seed {


EventWidget::EventWidget(const IWidget *sender, const IWidget *receiver, eType t, u32 p, f32 x, f32 y, u32 pressed, u32 hold, u32 released)
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
	iType = NONE;
}

INLINE const IWidget *EventWidget::GetSender() const
{
	return pSender;
}

INLINE const IWidget *EventWidget::GetReceiver() const
{
	return pReceiver;
}

INLINE EventWidget::eType EventWidget::GetEventType() const
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

INLINE const char *EventWidget::GetObjectName() const
{
	return "EventWidget";
}


} // namespace

