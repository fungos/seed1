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


class WidgetContainer : public IWidget
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
		virtual void Render(f32 delta);

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

