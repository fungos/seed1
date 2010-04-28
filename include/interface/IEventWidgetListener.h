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

class SEED_CORE_API IEventWidgetListener : public IEventListener
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
