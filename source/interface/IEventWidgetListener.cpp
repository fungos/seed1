/*! \file IEventWidgetListener.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event widget listener class interface
*/

#include "interface/IEventWidgetListener.h"

namespace Seed {

IEventWidgetListener::IEventWidgetListener()
{
}

IEventWidgetListener::~IEventWidgetListener()
{
}

INLINE void IEventWidgetListener::OnWidgetFocusLost(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetFocusReceived(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetRollOver(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetRollOut(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetPress(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetRelease(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetReleaseOut(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetDrag(const EventWidget *ev)
{
	UNUSED(ev);
}

INLINE void IEventWidgetListener::OnWidgetDrop(const EventWidget *ev)
{
	UNUSED (ev);
}

} // namespace
