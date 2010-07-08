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

/*! \file WidgetContainer.cpp
	\author Danny Angelo Carminati Grein
	\brief GUI WidgetContainer Widget
*/

#include "Defines.h"
#include "gui/WidgetContainer.h"
#include "gui/GuiManager.h"
#include "gui/EventWidget.h"
#include "Input.h"
#include "Log.h"
#include "Rect.h"
#include "Screen.h"
#include "Timer.h"
#include "Enum.h"
#include "SeedInit.h"
#include "RendererDevice.h"

#define TAG						"[GUI::WidgetContainer] "
#define DRAG_START_THRESHOLD	30

#if defined(SEED_LOG_GUIMANAGER)
	#define LOG		Log
#else
	#define LOG		_hid_gmlog
	void _hid_gmlog(const char *pMessage, ...);
	void _hid_gmlog(const char *pMessage, ...)
	{
		UNUSED(pMessage);
	}
#endif

namespace Seed {

WidgetContainer::WidgetContainer()
	: bEventConsumed(FALSE)
	, vWidget()
{
	this->Reset();
}

WidgetContainer::~WidgetContainer()
{
	this->Reset();

	if (Private::bInitialized)
		pGuiManager->Remove(this);
}

void WidgetContainer::Reset()
{
	iId				= 0;
	iColor.pixel	= 0;
	eBlendOperation	= BlendNone;

	IWidget::Reset();

	this->SetWidth(1.0f);
	this->SetHeight(1.0f);

	bVisible			= TRUE;
	bChanged			= TRUE;
	bTransformationChanged = TRUE;
	bDisabled			= FALSE;
	bEventConsumed	= FALSE;

	vWidget.clear();
	WidgetVector().swap(vWidget);
}

void WidgetContainer::Update(f32 dt)
{
	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();
	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		w->Update(dt);
	}

	std::sort(vWidget.begin(), vWidget.end(), ITransformable2DDescendingPrioritySort());
}

INLINE void WidgetContainer::SetVisible(BOOL b)
{
	IWidget::SetVisible(b);

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();
	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		w->SetVisible(b);
	}
}

INLINE void WidgetContainer::SetDisabled(BOOL b)
{
	IWidget::SetDisabled(b);

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();
	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		w->SetDisabled(b);
	}
}

INLINE void WidgetContainer::Render()
{
	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();
	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		w->Render();
	}

	if (pConfiguration->bDebugContainer)
		pRendererDevice->DrawRect(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), PIXEL_COLOR(255, 0, 255, 255));
}

INLINE void WidgetContainer::Add(IWidget *widget)
{
	ASSERT_NULL(widget);

	WidgetIterator p = std::find(vWidget.begin(), vWidget.end(), widget);
	if (p == vWidget.end())
	{
		//widget->SetParent(this);
		vWidget.push_back(widget);
		std::sort(vWidget.begin(), vWidget.end(), ITransformable2DDescendingPrioritySort());
	}
}

INLINE void WidgetContainer::Remove(IWidget *widget)
{
	ASSERT_NULL(widget);

	WidgetIterator p = std::find(vWidget.begin(), vWidget.end(), widget);
	if (p != vWidget.end())
	{
		//IWidget *widget = (*p);
		//widget->SetParent(NULL);

		vWidget.erase(p);
	}

	WidgetVector(vWidget).swap(vWidget);
}

INLINE BOOL WidgetContainer::IsEventConsumed() const
{
	return bEventConsumed;
}

void WidgetContainer::OnInputPointerPress(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();

	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		bEventConsumed = FALSE;

		if (w->IsDisabled())
			continue;

		f32 cX = ev->GetX();
		f32 cY = ev->GetY();

		if (!w->ContainsPoint(cX, cY))
			continue;

		if (w->GetObjectType() == Seed::ObjectGuiWidgetContainer)
		{
			WidgetContainer *wc = reinterpret_cast<WidgetContainer *>(w);
			wc->OnInputPointerPress(ev);

			bEventConsumed = wc->IsEventConsumed();
			if (bEventConsumed)
				break;
		}

		if (!(w->GetTrigger() & ev->GetPressed()))
			continue;

		u32 j = ev->GetJoystick();

		//LOG("> PRESS [%d, %f, %f]", j, cX, cY);

		if (w->GetState() == Seed::WidgetStateDrag)
		{
			continue;
		}

		const EventWidget newEvent(w, NULL, WidgetEventPressed, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

		LOG(">PRESSED_OVER [id: %d]", w->GetId());
		// GetState eh o baseado em prioridade a partir de todos os inputs.
		if (w->GetState() != Seed::WidgetStatePressedOver && w->GetPlayerState(j) != Seed::WidgetStatePressedOver)
		{
			// muda apenas o estado interno do widget
			LOG("\tEstado WIDGET");
			w->OnWidgetPress(&newEvent);
		}

		w->SetState(Seed::WidgetStatePressed);
		w->SetPlayerState(Seed::WidgetStatePressedOver, j);
		LOG("\tEstado PLAYER");

		w->SendOnPress(&newEvent);
		LOG("\tEvento");
		//if (newEvent.IsConsumed())
		bEventConsumed = newEvent.IsConsumed();
		if (bEventConsumed)
			break;
	}
}

void WidgetContainer::OnInputPointerRelease(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();

	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		//BOOL consumed = FALSE;

		bEventConsumed = FALSE;

		ASSERT_NULL(w);

		if (w->IsDisabled())
			continue;

		if (w->GetObjectType() == Seed::ObjectGuiWidgetContainer)
		{
			WidgetContainer *wc = reinterpret_cast<WidgetContainer *>(w);
			wc->OnInputPointerRelease(ev);

			bEventConsumed = wc->IsEventConsumed();
			if (bEventConsumed)
				break;
		}

		//f32 cX = ev->GetX();
		//f32 cY = ev->GetY();
		u32 j = ev->GetJoystick();

		if (!(w->GetTrigger() & ev->GetReleased()))
			continue;

		if (w->GetState() == Seed::WidgetStateDrag)
		{
			//LOG("> STOP DRAG [%d, %f, %f]", j, cX, cY);
			bEventConsumed = this->DoDrop(ev, w);
		}
		else
		{
			if (w->GetPlayerState(j) == Seed::WidgetStatePressedOver)
			{
				//LOG("> RELEASE [%d, %f, %f]", j, cX, cY);
				//consumed = this->DoRelease(ev, w);
				bEventConsumed = this->DoRelease(ev, w);
			}
			else if (w->GetPlayerState(j) == Seed::WidgetStatePressedOut)
			{
				//LOG("> RELEASE OUT [%d, %f, %f]", j, cX, cY);
				//consumed = this->DoReleaseOut(ev, w);
				bEventConsumed = this->DoReleaseOut(ev, w);
			}
		}

		//if (consumed)
		if (bEventConsumed)
			break;
	}
}

void WidgetContainer::OnInputPointerMove(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();

	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		bEventConsumed = FALSE;

		if (w->IsDisabled())
			continue;

		if (w->GetObjectType() == Seed::ObjectGuiWidgetContainer)
		{
			WidgetContainer *wc = reinterpret_cast<WidgetContainer *>(w);
			wc->OnInputPointerMove(ev);

			bEventConsumed = wc->IsEventConsumed();
			if (bEventConsumed)
				break;
		}

		bEventConsumed = this->UpdateStates(ev, w);
		//if (UpdateStates(ev, w))
		if (bEventConsumed)
			break;
	}
}

BOOL WidgetContainer::DoDrag(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();

	IWidget *receiver = NULL;
	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		/* FIX ME */
		w->SetPlayerState(Seed::WidgetStateDrag, j);
		/* FIX ME */
		if (w != widget && w->ContainsPoint(pInput->GetX(), pInput->GetY()) && !w->IsDisabled())
		{
			receiver = w;
			//break;

			const EventWidget newEvent(widget, receiver, WidgetEventDrag, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

			LOG(">DRAG OVER [id: %d]", receiver->GetId());
			LOG("\tEstado WIDGET");
			widget->SetState(Seed::WidgetStateDrag);
			widget->OnWidgetDrag(&newEvent);

			widget->SendOnDrag(&newEvent);
			LOG("\tEvento");
			if (newEvent.IsConsumed())
				return TRUE;
		}
	}

	if (!receiver)
	{
		const EventWidget newEvent(widget, NULL, WidgetEventDrag, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

		LOG(">DRAG [id: %d]", widget->GetId());
		LOG("\tEstado WIDGET");
		widget->SetState(Seed::WidgetStateDrag);
		widget->OnWidgetDrag(&newEvent);

		widget->SendOnDrag(&newEvent);
		LOG("\tEvento");
		if (newEvent.IsConsumed())
			return TRUE;
	}

	return FALSE;
}

BOOL WidgetContainer::DoDrop(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();

	WidgetIterator it = vWidget.begin();
	WidgetIterator end = vWidget.end();

	IWidget *receiver = NULL;
	for (; it != end; ++it)
	{
		IWidget *w = (*it);
		ASSERT_NULL(w);

		/* FIX ME */
		w->SetPlayerState(Seed::WidgetStateNone, j);
		/* FIX ME */

		if (w != widget && w->ContainsPoint(pInput->GetX(), pInput->GetY()) && !w->IsDisabled())
		{
			receiver = w;
			//break;

			const EventWidget newEvent(widget, receiver, WidgetEventDrop, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

			LOG(">DROP OVER [id: %d]", widget->GetId());
			LOG("\tEstado WIDGET");
			widget->SetState(Seed::WidgetStateOver);
			widget->OnWidgetDrop(&newEvent);

			LOG("\tEstado PLAYER");
			widget->SetPlayerState(Seed::WidgetStateOver, j);

			widget->SendOnDrop(&newEvent);
			LOG("\tEvento");
			if (newEvent.IsConsumed())
				return TRUE;
		}
	}

	if (!receiver)
	{
		const EventWidget newEvent(widget, NULL, WidgetEventDrop, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

		LOG(">DROP [id: %d]", widget->GetId());
		LOG("\tEstado WIDGET");
		widget->SetState(Seed::WidgetStateOver);
		widget->OnWidgetDrop(&newEvent);

		LOG("\tEstado PLAYER");
		widget->SetPlayerState(Seed::WidgetStateOver, j);

		widget->SendOnDrop(&newEvent);
		LOG("\tEvento");
		if (newEvent.IsConsumed())
			return TRUE;
	}

	return FALSE;
}

BOOL WidgetContainer::DoReleaseOut(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();

	const EventWidget newEvent(widget, NULL, WidgetEventReleasedOut, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

	u32 count = 0;

	// count how many players in any PRESSED_* state exists
	for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		eWidgetState s = widget->GetPlayerState(i);
		if (s == Seed::WidgetStatePressedOver || s == Seed::WidgetStatePressedOut)
			count++;
	}

	LOG("\tRELEASE OUT [id: %d]", widget->GetId());
	// if its the only PRESSED_* we can change the state to NONE
	if (count == 1)
	{
		LOG("\tEstado WIDGET");
		widget->SetState(Seed::WidgetStateNone);
		widget->OnWidgetReleaseOut(&newEvent);
	}

	LOG("\tEstado PLAYER");
	widget->SetPlayerState(Seed::WidgetStateNone, j);

	widget->SendOnReleaseOut(&newEvent);
	LOG("\tEvento");
	return newEvent.IsConsumed();
}

BOOL WidgetContainer::DoRelease(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();
	//LOG("> RELEASE OVER [%d, %f, %f]", j, cX, cY);

	const EventWidget newEvent(widget, NULL, WidgetEventReleased, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

	u32 count = 0;

	// count how many players in PRESSED_OVER exists
	for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		eWidgetState s = widget->GetPlayerState(i);
		if (s == Seed::WidgetStatePressedOver || s == Seed::WidgetStatePressedOut)
			count++;
	}

	LOG(">RELEASE [id: %d]", widget->GetId());
	// if its the only PRESSED_OVER we can change the state to OVER here
	if (count == 1)
	{
		//LOG("- RELEASE OVER SetState OVER");
		LOG("\tEstado WIDGET");
		widget->SetState(Seed::WidgetStateOver);
		widget->OnWidgetRelease(&newEvent);
	}

	LOG("\tEstado PLAYER");
	widget->SetPlayerState(Seed::WidgetStateOver, j);

	widget->SendOnRelease(&newEvent);
	LOG("\tEvento");
	return newEvent.IsConsumed();
}

BOOL WidgetContainer::DoOut(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();
	//LOG("> OUT [%d, %f, %f]", j, cX, cY);

	const EventWidget newEvent(widget, NULL, WidgetEventOut, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

	u32 count = 0;

	for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		eWidgetState s = widget->GetPlayerState(i);
		if (s == Seed::WidgetStateOver || s == Seed::WidgetStatePressedOver || s == Seed::WidgetStatePressedOut)
			count++;
	}

	LOG(">ROLL OUT [id: %d]", widget->GetId());
	if (count == 1)
	{
		LOG("\tEstado WIDGET");
		widget->SetState(Seed::WidgetStateNone);

		// dispatch event to widget manage its internal state
		widget->OnWidgetRollOut(&newEvent);
	}

	LOG("\tEstado PLAYER");
	widget->SetPlayerState(Seed::WidgetStateNone, j);

	// dispatch event to widget listeners
	widget->SendOnRollOut(&newEvent);
	LOG("\tEvento");

	return ev->IsConsumed();
}

BOOL WidgetContainer::DoOver(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();

	//LOG("> OVER [%d, %f, %f]", j, cX, cY);

	LOG("> ROLL OVER [id: %d]", widget->GetId());
	if (widget->GetPlayerState(j) == Seed::WidgetStatePressedOut)
	{
		//LOG("- %d OVER - PRESSED OUT", j);
		LOG("\tEstado PLAYER");
		widget->SetPlayerState(Seed::WidgetStatePressedOver, j);
		widget->SetState(Seed::WidgetStatePressed);
	}

	if (widget->GetPlayerState(j) == Seed::WidgetStatePressedOver)
	{
		//LOG("- %d OVER - PRESSED OVER", j);
		return FALSE;
	}

	const EventWidget newEvent(widget, NULL, WidgetEventOver, j, cX, cY, ev->GetPressed(), ev->GetHold(), ev->GetReleased());

	if (widget->GetState() == Seed::WidgetStateNone)
	{
		//LOG("- %d OVER - State == NONE", j);
		// dispatch event to widget manage its internal state
		LOG("\tEstado WIDGET");
		widget->OnWidgetRollOver(&newEvent);
		widget->SetState(Seed::WidgetStateOver);
	}

	// to guarantee that we will send only one over event by player to widget
	if (widget->GetPlayerState(j) == Seed::WidgetStateNone)
	{
		//LOG("- %d OVER - PlayerState == NONE");
		LOG("\tEstado PLAYER");
		widget->SetPlayerState(Seed::WidgetStateOver, j);

		// dispatch event to widget listeners
		widget->SendOnRollOver(&newEvent);
		LOG("\tEvento");
		if (newEvent.IsConsumed())
			return TRUE;
	}
	return FALSE;
}

BOOL WidgetContainer::UpdateStates(const EventInputPointer *ev, IWidget *widget)
{
	f32 cX = ev->GetX();
	f32 cY = ev->GetY();
	u32 j = ev->GetJoystick();

	BOOL ret = FALSE;

	// Testa casos onde um cursor esta dentro do Widget
	if ((widget->GetState() == Seed::WidgetStateDrag && widget->GetPlayerState(j) == Seed::WidgetStateDrag))
	{
		ret = this->DoDrag(ev, widget);
	}
	else if (widget->ContainsPoint(cX, cY))
	{
		if (widget->GetState() == Seed::WidgetStateNone && widget->GetPlayerState(j) == Seed::WidgetStateNone)
		{
			//LOG("> OVER [%d, %f, %f]", j, cX, cY);
			ret = this->DoOver(ev, widget);
		}
		else if (widget->GetState() == Seed::WidgetStatePressed)
		{
			if (widget->GetPlayerState(j) != Seed::WidgetStatePressedOver)// && widget->GetPlayerState(j) != Seed::WidgetStatePressedOut)
			{
				//LOG("> PRESSED OVER [%d, %f, %f]", j, cX, cY);
				ret = this->DoOver(ev, widget);
			}
			else if (widget->IsDraggable() && (pTimer->GetMilliseconds() - widget->GetStateStartTime()) > DRAG_START_THRESHOLD)
			{
				LOG("\t\tPlayerState %d: %s", (int)widget->GetPlayerState(j), widget->GetPlayerState(j) == Seed::WidgetStateNone ? "NONE" : widget->GetPlayerState(j) == Seed::WidgetStatePressedOver ? "PRESS_OVER" : widget->GetPlayerState(j) == Seed::WidgetStatePressedOut ? "PRESS_OUT" : widget->GetPlayerState(j) == Seed::WidgetStateOver ? "ROLLOVER" : "OTHER");
				ret = this->DoDrag(ev, widget);
			}
		}
	}
	else
	{
		if (widget->GetPlayerState(j) == Seed::WidgetStateOver)// && !bOutConsumed)
		{
			//LOG("> OUT [%d, %f, %f]", j, cX, cY);
			ret = this->DoOut(ev, widget);
		}
		// Not in widget rectangle and was pressed then we keep it as a pressed out state.
		else if (widget->GetPlayerState(j) == Seed::WidgetStatePressedOver)
		{
			if (widget->IsDraggable())
			{
				//LOG("> DRAG [%d, %f, %f]", j, cX, cY);
				ret = this->DoDrag(ev, widget);
			}
			else
			{
				//LOG("> PRESSED OUT [%d, %f, %f]", j, cX, cY);
				//LOG("> PRESSED OUT [id: %d]", widget->GetId());
				widget->SetPlayerState(Seed::WidgetStatePressedOut, j);
			}
		}
	}

	return ret;
}

INLINE const char *WidgetContainer::GetObjectName() const
{
	return "WidgetContainer";
}

INLINE int WidgetContainer::GetObjectType() const
{
	return Seed::ObjectGuiWidgetContainer;
}

} // namespace
