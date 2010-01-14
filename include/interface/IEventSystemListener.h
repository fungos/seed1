/*! \file IEventSystemListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event system listener class interface
*/


#ifndef __IEVENTSYSTEMLISTENER_H__
#define __IEVENTSYSTEMLISTENER_H__

#include "IEventListener.h"
#include "Defines.h"


namespace Seed {


class EventSystem;


class IEventSystemListener : public IEventListener
{
	public:
		IEventSystemListener();
		virtual ~IEventSystemListener();

		virtual void OnSystemReset(const EventSystem *ev);
		virtual void OnSystemShutdown(const EventSystem *ev);
		virtual void OnSystemMenuCalled(const EventSystem *ev);
		virtual void OnSystemDataManagerCalled(const EventSystem *ev);
		virtual void OnSystemHomeEnded(const EventSystem *ev);
		virtual void OnSystemSleep(const EventSystem *ev);
		virtual void OnSystemLanguageChanged(const EventSystem *ev);

	private:
		SEED_DISABLE_COPY(IEventSystemListener);
};


} // namespace


#endif // __IEVENTSYSTEMLISTENER_H__
