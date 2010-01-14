/*! \file IEventInputPointerListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event pointer listener class interface
*/


#ifndef __IEVENT_INPUT_POINTER_LISTENER_H__
#define __IEVENT_INPUT_POINTER_LISTENER_H__


#include "interface/IEventListener.h"
#include "EventInputPointer.h"
#include "EventInputMotion.h"


namespace Seed {


class IEventInputPointerListener : public IEventListener
{
	public:
		IEventInputPointerListener();
		virtual ~IEventInputPointerListener();

		virtual void OnInputPointerPress(const EventInputPointer *ev);
		virtual void OnInputPointerRelease(const EventInputPointer *ev);
		virtual void OnInputPointerMove(const EventInputPointer *ev);
		virtual void OnInputPointerEnable(const EventInputPointer *ev);
		virtual void OnInputPointerDisable(const EventInputPointer *ev);

		//FIXME remove
		virtual void OnAccelerationChanged(const EventInputMotion *ev);

	private:
		SEED_DISABLE_COPY(IEventInputPointerListener);
};


} // namespace


#endif // __IEVENT_INPUT_POINTER_LISTENER_H__

