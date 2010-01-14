/*! \file IWidget.h
	\author Danny Angelo Carminati Grein
	\brief GUI Base Widget
*/

#ifndef __GUI_WIDGET_H__
#define __GUI_WIDGET_H__

#include "Defines.h"
#include "interface/IEventWidgetListener.h"
#include "interface/ITransformable2D.h"
#include "interface/IInput.h"

#include <vector>
#include <algorithm>


namespace Seed {

class WidgetContainer;

class IWidget : public ITransformable2D, public IEventWidgetListener
{
	public:
		enum eState
		{
			NONE,
			FOCUSED,
			OVER,
			PRESSED,
			PRESSED_OVER,
			PRESSED_OUT,
			DRAG
		};

	public:
		IWidget();
		IWidget(u32 id, f32 w, f32 h, f32 x, f32 y);
		virtual ~IWidget();

		virtual void Update(f32 dt);
		virtual void SetTrigger(eInputButton button);
		virtual eInputButton GetTrigger();

		virtual void SetId(u32 iId);
		virtual u32 GetId() const;

		virtual void SetSelected(BOOL b);
		virtual BOOL IsSelected() const;

		virtual void SetDisabled(BOOL b);
		virtual BOOL IsDisabled() const;

		virtual void SetDraggable(BOOL b);
		virtual BOOL IsDraggable() const;

		void SetState(IWidget::eState s);
		IWidget::eState GetState() const;

		void SetPlayerState(IWidget::eState s, u32 i = 0);
		IWidget::eState GetPlayerState(u32 i = 0) const;

		u64 GetPlayerStateStartTime(u32 i) const;
		u64 GetStateStartTime() const;

		void AddListener(IEventWidgetListener *listener);
		void RemoveListener(IEventWidgetListener *listener);

		// ITransformable 2D
		virtual void SetVisible(BOOL b);

		// IEventWidgetListener
		virtual void SendOnRollOver(const EventWidget *ev);
		virtual void SendOnRollOut(const EventWidget *ev);
		virtual void SendOnPress(const EventWidget *ev);
		virtual void SendOnRelease(const EventWidget *ev);
		virtual void SendOnReleaseOut(const EventWidget *ev);
		virtual void SendOnDrop(const EventWidget *ev);
		virtual void SendOnDrag(const EventWidget *ev);

	protected:
		void Reset();

	protected:
		typedef std::vector<IEventWidgetListener *> 	IEventWidgetListenerVector;
		typedef IEventWidgetListenerVector::iterator 	IEventWidgetListenerIterator;

		IEventWidgetListenerVector vListener;

		u32		iId;
		u64		iStateStartTime;

		BOOL	bDisabled;
		BOOL	bSelected;
		BOOL	bChanged;
		BOOL	bDraggable;

		IWidget::eState 	iState;
		IWidget::eState 	iPlayerState[PLATFORM_MAX_INPUT];
		u64					arPlayerStateStartTime[PLATFORM_MAX_INPUT];

		Seed::eInputButton iButton;

	private:
		SEED_DISABLE_COPY(IWidget);
};


} // namespace


#endif // __IWIDGET_H__
