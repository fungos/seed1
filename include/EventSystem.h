/*! \file EventSystem.h
	\author	Danny Angelo Carminati Grein
	\brief A system event
*/


#ifndef __EVENTSYSTEM_H__
#define __EVENTSYSTEM_H__


#include "interface/IEvent.h"
#include "Enum.h"


namespace Seed {


class EventSystem : public IEvent
{
	public:
		EventSystem();
		EventSystem(Seed::eLanguage curLang, Seed::eLanguage newLang);

		virtual ~EventSystem();

		virtual Seed::eLanguage GetLanguageCurrent() const;
		virtual Seed::eLanguage GetLanguageNew() const;

		// IObject
		virtual const char *GetObjectName() const;

	private:
		Seed::eLanguage nCurrentLang;
		Seed::eLanguage nNewLang;

	private:
		SEED_DISABLE_COPY(EventSystem);
};


} // namespace


#endif // __EVENTSYSTEM_H__
