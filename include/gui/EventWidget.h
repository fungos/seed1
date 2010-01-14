/*! \file EventWidget.h
	\author	Danny Angelo Carminati Grein
	\brief Defines a widget event class
*/


#ifndef __EVENT_WIDGET_H__
#define __EVENT_WIDGET_H__


#include "interface/IEvent.h"


namespace Seed {


class IWidget;


class EventWidget : public IEvent
{
	public:
		enum eType
		{
			NONE,
			FOCUS_LOST,
			FOCUS_RECEIVED,
			OVER,
			OUT,
			PRESSED_OUT,
			RELEASED_OUT,
			PRESSED,
			RELEASED,
			DRAG,
			DROP
		};

	public:
		virtual ~EventWidget();
		EventWidget(const IWidget *sender, const IWidget *receiver, eType t, u32 p, f32 x, f32 y, u32 pressed, u32 hold, u32 released);

		const IWidget *GetSender() const;
		const IWidget *GetReceiver() const;
		EventWidget::eType GetEventType() const;
		f32 GetX() const;
		f32 GetY() const;
		u32 GetPlayer() const;
		u32 GetPressed()  const;
		u32 GetReleased() const;
		u32 GetHold() 	 const;

		// IObject
		virtual const char *GetObjectName() const;

	private:
		SEED_DISABLE_COPY(EventWidget);

	private:
		const IWidget *pSender;
		const IWidget *pReceiver;
		eType iType;
		u32 iPlayer;
		f32 fX;
		f32 fY;
		u32 iPressed;
		u32 iHold;
		u32 iReleased;
};


} // namespace


#endif // __EVENT_WIDGET_H__

