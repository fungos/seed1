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

/*! \file WidgetContainer.h
	\author Danny Angelo Carminati Grein
	\brief GUI WidgetContainer Widget
*/

#ifndef __GUI_WIDGET_CONTAINER_H__
#define __GUI_WIDGET_CONTAINER_H__

#include "Array.h"
#include "../interface/IWidget.h"
#include "interface/IEventInputPointerListener.h"
#include <vector>

namespace Seed {

class SEED_CORE_API WidgetContainer : public IWidget
{
	public:
		WidgetContainer();
		virtual ~WidgetContainer();

		virtual void Reset();
		virtual void Update(f32 dt);

		virtual void OnInputPointerPress(const EventInputPointer *ev);
		virtual void OnInputPointerRelease(const EventInputPointer *ev);
		virtual void OnInputPointerMove(const EventInputPointer *ev);

		virtual void Add(IWidget *widget);
		virtual void Remove(IWidget *widget);

		virtual BOOL IsEventConsumed() const;

		// ITransformable2D
		virtual void SetDisabled(BOOL b);

		// IRenderable
		virtual void SetVisible(BOOL b);
		virtual void Render();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(WidgetContainer);

		BOOL DoOut(const EventInputPointer *ev, IWidget *widget);
		BOOL DoOver(const EventInputPointer *ev, IWidget *widget);
		BOOL DoDrag(const EventInputPointer *ev, IWidget *widget);
		BOOL DoRelease(const EventInputPointer *ev, IWidget *widget);
		BOOL DoReleaseOut(const EventInputPointer *ev, IWidget *widget);
		BOOL DoDrop(const EventInputPointer *ev, IWidget *widget);

		BOOL UpdateStates(const EventInputPointer *ev, IWidget *widget);

	private:
		typedef std::vector<IWidget *> WidgetVector;
		typedef WidgetVector::iterator WidgetIterator;

		BOOL bEventConsumed;

		WidgetVector vWidget;
};

} // namespace

#endif // __GUI_WIDGET_CONTAINER_H__
