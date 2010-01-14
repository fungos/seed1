/*! \file EventSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief A system event
*/


#include "EventSystem.h"


namespace Seed {


EventSystem::EventSystem()
	: IEvent(SYSTEM, EVENT_SYSTEM)
	, nCurrentLang(Seed::en_US)
	, nNewLang(Seed::en_US)
{
}

EventSystem::EventSystem(Seed::eLanguage curLang, Seed::eLanguage newLang)
	: IEvent(SYSTEM, EVENT_SYSTEM)
	, nCurrentLang(curLang)
	, nNewLang(newLang)
{
}

EventSystem::~EventSystem()
{
}

INLINE Seed::eLanguage EventSystem::GetLanguageCurrent() const
{
	return this->nCurrentLang;
}

INLINE Seed::eLanguage EventSystem::GetLanguageNew() const
{
	return this->nNewLang;
}

INLINE const char *EventSystem::GetObjectName() const
{
	return "EventSystem";
}


} // namespace
