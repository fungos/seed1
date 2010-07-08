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

/*! \file EventWidget.h
	\author	Danny Angelo Carminati Grein
	\brief Defines a widget event class
*/

#ifndef __EVENT_WIDGET_H__
#define __EVENT_WIDGET_H__

#include "interface/IEvent.h"

namespace Seed {

class IWidget;

class SEED_CORE_API EventWidget : public IEvent
{
	public:
		virtual ~EventWidget();
		EventWidget(const IWidget *sender, const IWidget *receiver, eWidgetEventType t, u32 p, f32 x, f32 y, u32 pressed, u32 hold, u32 released);

		const IWidget *GetSender() const;
		const IWidget *GetReceiver() const;
		eWidgetEventType GetEventType() const;

		f32 GetX() const;
		f32 GetY() const;
		u32 GetPlayer() const;
		u32 GetPressed()  const;
		u32 GetReleased() const;
		u32 GetHold() 	 const;

	private:
		SEED_DISABLE_COPY(EventWidget);

	private:
		const IWidget *pSender;
		const IWidget *pReceiver;
		eWidgetEventType iType;
		u32 iPlayer;
		f32 fX;
		f32 fY;
		u32 iPressed;
		u32 iHold;
		u32 iReleased;
};

} // namespace

#endif // __EVENT_WIDGET_H__
