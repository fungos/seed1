/*! \file IEventWidgetListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event widget listener class interface
*/


#ifndef __IEVENT_WIDGET_LISTENER_H__
#define __IEVENT_WIDGET_LISTENER_H__


#include "interface/IEventListener.h"
#include "interface/IEvent.h"


namespace Seed {


class EventWidget;


class IEventWidgetListener : public IEventListener
{
	public:
		IEventWidgetListener();
		virtual ~IEventWidgetListener();

		virtual void OnWidgetFocusLost(const EventWidget *ev);
		virtual void OnWidgetFocusReceived(const EventWidget *ev);
		virtual void OnWidgetRollOver(const EventWidget *ev);
		virtual void OnWidgetRollOut(const EventWidget *ev);
		virtual void OnWidgetPress(const EventWidget *ev);
		virtual void OnWidgetRelease(const EventWidget *ev);
		virtual void OnWidgetReleaseOut(const EventWidget *ev);
		virtual void OnWidgetDrag(const EventWidget *ev);
		virtual void OnWidgetDrop(const EventWidget *ev);

	private:
		SEED_DISABLE_COPY(IEventWidgetListener);
};


} // namespace


#endif // __IEVENT_WIDGET_LISTENER_H__

