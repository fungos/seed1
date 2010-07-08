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

/*! \file IWidget.cpp
	\author Danny Angelo Carminati Grein
	\brief GUI Base Widget
*/

#include "interface/IWidget.h"
#include "Log.h"
#include "Timer.h"
#include "gui/EventWidget.h"

#include <vector>
#include <algorithm>

namespace Seed {

IWidget::IWidget()
	: ISceneObject()
	, IEventWidgetListener()
	, vListener()
	, iId(0)
	, iStateStartTime(0)
	, bDisabled(FALSE)
	, bSelected(FALSE)
	, bChanged(TRUE)
	, bDraggable(FALSE)
	, iState(Seed::WidgetStateNone)
	, iButton(Seed::ButtonAll)
{
	for (int i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		iPlayerState[i] = Seed::WidgetStateNone;
		arPlayerStateStartTime[i] = 0;
	}
}

IWidget::IWidget(u32 id, f32 w, f32 h, f32 x, f32 y)
	: ISceneObject()
	, IEventWidgetListener()
	, vListener()
	, iId(id)
	, iStateStartTime(0)
	, bDisabled(FALSE)
	, bSelected(FALSE)
	, bChanged(TRUE)
	, bDraggable(FALSE)
	, iState(Seed::WidgetStateNone)
	, iButton(Seed::ButtonAll)
{
	this->SetWidth(w);
	this->SetHeight(h);
	this->SetX(x);
	this->SetY(y);

	for (int i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		iPlayerState[i] = Seed::WidgetStateNone;
		arPlayerStateStartTime[i] = 0;
	}
}

IWidget::~IWidget()
{
	this->Reset();
}

void IWidget::Reset()
{
	this->iId = 0;
	this->iStateStartTime = 0;
	this->bDisabled = TRUE;
	this->bDraggable = FALSE;
	this->iButton = Seed::ButtonAll;
	this->bChanged	= FALSE;
	ITransformable2D::Reset();
}

void IWidget::Update(f32 dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;
}

INLINE void IWidget::SetTrigger(eInputButton button)
{
	this->iButton = button;
}

INLINE eInputButton IWidget::GetTrigger()
{
	return this->iButton;
}

/*
TODO: 04-11-2008 | DESIGN | MINOR | FEATURE | All widgets must be inside a container | Danny Angelo Carminati Grein
TODO: 04-11-2008 | DESIGN | MINOR | FEATURE | The widget position x,y must be relative to its parent container | Danny Angelo Carminati Grein
*/
//INLINE void IWidget::SetX(f32 x)
INLINE void IWidget::SetId(u32 iId)
{
	this->iId = iId;
}

INLINE u32 IWidget::GetId() const
{
	return this->iId;
}

INLINE void IWidget::SetSelected(BOOL b)
{
	this->bSelected = b;
}

INLINE BOOL IWidget::IsSelected() const
{
	return this->bSelected;
}

INLINE void IWidget::SetDisabled(BOOL b)
{
	this->bDisabled = b;
}

INLINE BOOL IWidget::IsDisabled() const
{
	return this->bDisabled;
}

INLINE void IWidget::SetDraggable(BOOL b)
{
	this->bDraggable = b;
}

INLINE BOOL IWidget::IsDraggable() const
{
	return this->bDraggable;
}

INLINE void IWidget::SetState(eWidgetState s)
{
	this->iStateStartTime = pTimer->GetMilliseconds();
	this->iState = s;
}

INLINE eWidgetState IWidget::GetState() const
{
	return this->iState;
}

INLINE void IWidget::SetPlayerState(eWidgetState s, u32 i)
{
	this->arPlayerStateStartTime[i] = pTimer->GetMilliseconds();
	this->iPlayerState[i] = s;
}

INLINE eWidgetState IWidget::GetPlayerState(u32 i) const
{
	return this->iPlayerState[i];
}

INLINE u64 IWidget::GetPlayerStateStartTime(u32 i) const
{
	return arPlayerStateStartTime[i];
}

INLINE u64 IWidget::GetStateStartTime() const
{
	return iStateStartTime;
}

void IWidget::AddListener(IEventWidgetListener *listener)
{
	ASSERT_NULL(listener);
	vListener.push_back(listener);
}

INLINE void IWidget::SetVisible(BOOL b)
{
	IRenderable::SetVisible(b);
	if (!b)
	{
		this->iState = Seed::WidgetStateNone;
		this->iStateStartTime = 0;

		for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
		{
			this->iPlayerState[i] = Seed::WidgetStateNone;
			this->arPlayerStateStartTime[i] = 0;
		}
	}
}

void IWidget::RemoveListener(IEventWidgetListener *listener)
{
	ASSERT_NULL(listener);
	IEventWidgetListenerIterator p = std::find(vListener.begin(), vListener.end(), listener);

	if (p != vListener.end())
	{
		vListener.erase(p);
	}

	if (vListener.size() == 0)
	{
		std::vector<IEventWidgetListener *>().swap( vListener );
	}
}

INLINE void IWidget::SendOnRollOver(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetRollOver(ev);
		if (ev->IsConsumed())
		{
			break;
		}
	}
}

INLINE void IWidget::SendOnRollOut(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetRollOut(ev);
		if (ev->IsConsumed())
			break;
	}
}

INLINE void IWidget::SendOnPress(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetPress(ev);
		if (ev->IsConsumed())
			break;
	}
}

INLINE void IWidget::SendOnDrag(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetDrag(ev);
		if (ev->IsConsumed())
			break;
	}
}

INLINE void IWidget::SendOnDrop(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		if (ev->IsConsumed())
			break;

		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetDrop(ev);
	}
}

INLINE void IWidget::SendOnRelease(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetRelease(ev);
		if (ev->IsConsumed())
			break;
	}
}

INLINE void IWidget::SendOnReleaseOut(const EventWidget *ev)
{
	ASSERT_NULL(ev);

	IEventWidgetListenerIterator it = vListener.begin();
	IEventWidgetListenerIterator end = vListener.end();

	for (; it != end; ++it)
	{
		IEventWidgetListener *obj = (*it);
		ASSERT_NULL(obj);
		obj->OnWidgetReleaseOut(ev);
		if (ev->IsConsumed())
			break;
	}
}

INLINE const char *IWidget::GetObjectName() const
{
	return "Widget";
}

INLINE int IWidget::GetObjectType() const
{
	return Seed::ObjectGuiWidget;
}

} // namespace
