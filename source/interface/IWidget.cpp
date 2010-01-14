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
	: vListener()
	, iId(0)
	, iStateStartTime(0)
	, bDisabled(FALSE)
	, bSelected(FALSE)
	, bChanged(TRUE)
	, bDraggable(FALSE)
	, iState(NONE)
	, iButton(Seed::ButtonAll)
{
	for (int i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		iPlayerState[i] = NONE;
		arPlayerStateStartTime[i] = 0;
	}
}


IWidget::IWidget(u32 id, f32 w, f32 h, f32 x, f32 y)
	: ITransformable2D()
	, vListener()
	, iId(id)
	, iStateStartTime(0)
	, bDisabled(FALSE)
	, bSelected(FALSE)
	, bChanged(TRUE)
	, bDraggable(FALSE)
	, iState(NONE)
	, iButton(Seed::ButtonAll)
{
	this->SetWidth(w);
	this->SetHeight(h);
	this->SetX(x);
	this->SetY(y);

	for (int i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		iPlayerState[i] = NONE;
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

INLINE void IWidget::SetState(eState s)
{
	this->iStateStartTime = pTimer->GetMilliseconds();
	this->iState = s;
}

INLINE IWidget::eState IWidget::GetState() const
{
	return this->iState;
}

INLINE void IWidget::SetPlayerState(IWidget::eState s, u32 i)
{
	this->arPlayerStateStartTime[i] = pTimer->GetMilliseconds();
	this->iPlayerState[i] = s;
}

INLINE IWidget::eState IWidget::GetPlayerState(u32 i) const
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
	ITransformable2D::SetVisible(b);
	if (!b)
	{
		this->iState = IWidget::NONE;
		this->iStateStartTime = 0;

		for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
		{
			this->iPlayerState[i] = IWidget::NONE;
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

} // namespace
