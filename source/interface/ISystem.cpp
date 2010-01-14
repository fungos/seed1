/*! \file ISystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the System class interface
*/


#include "interface/ISystem.h"
#include "interface/IEventSystemListener.h"
#include "EventSystem.h"


namespace Seed {


ISystem::ISystem()
	: vListeners()
	, pStrAppName(NULL)
	, pStrAppDescription(NULL)
	, nLanguage(Seed::en_US)
	, bDefaultCursorEnabled(FALSE)
	, iRetraceIndex(0)
{
	MEMSET(arRetraceCount, '\0', sizeof(arRetraceCount));
}

ISystem::~ISystem()
{
}

INLINE void ISystem::SetApplicationTitle(const char *title)
{
	this->pStrAppName = title;
}

INLINE const char *ISystem::GetApplicationTitle() const
{
	return this->pStrAppName;
}

INLINE void ISystem::SetLanguage(Seed::eLanguage lang)
{
	if (this->nLanguage != lang)
	{
		EventSystem ev(this->nLanguage, lang);

		this->nLanguage = lang;
		this->SendEventLanguageChanged(&ev);
	}
}

INLINE Seed::eLanguage ISystem::GetLanguage() const
{
	return this->nLanguage;
}

const char *ISystem::GetLanguageString() const
{
	const char *lang = "en_US";
	switch (this->nLanguage)
	{
		case Seed::pt_BR:
		{
			lang = "pt_BR";
		}
		break;

		case Seed::es_ES:
		{
			lang = "es_ES";
		}
		break;

		case Seed::ja_JA:
		{
			lang = "ja_JA";
		}
		break;

		case Seed::de_DE:
		{
			lang = "de_DE";
		}
		break;

		case Seed::cn_CN:
		{
			lang = "cn_CN";
		}
		break;

		default:
		break;
	}

	return lang;
}

INLINE const char *ISystem::GetUsername() const
{
	return NULL;
}

INLINE const char *ISystem::GetHomeFolder() const
{
	return NULL;
}

INLINE const char *ISystem::GetApplicationDataFolder() const
{
	return NULL;
}

INLINE const char *ISystem::GetSaveGameFolder() const
{
	return NULL;
}

INLINE void ISystem::EnableDefaultCursor(BOOL b)
{
	this->bDefaultCursorEnabled = b;
}

INLINE BOOL ISystem::IsDefaultCursorEnabled() const
{
	return bDefaultCursorEnabled;
}

INLINE void ISystem::SetApplicationDescription(const char *descr)
{
	this->pStrAppDescription = descr;
}

INLINE const char *ISystem::GetApplicationDescription() const
{
	return this->pStrAppDescription;
}

INLINE void ISystem::AddListener(IEventSystemListener *listener)
{
	ASSERT_NULL(listener);
	vListeners.push_back(listener);
}

INLINE void ISystem::RemoveListener(IEventSystemListener *listener)
{
	ASSERT_NULL(listener);
	ListenerIterator p = std::find(vListeners.begin(), vListeners.end(), listener);

	if (p != vListeners.end())
		vListeners.erase(p);

	ListenerVector(vListeners).swap(vListeners);
}

INLINE void ISystem::SendEventReset(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemReset(ev);
	}
}

INLINE void ISystem::SendEventShutdown(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemShutdown(ev);
	}
}

INLINE void ISystem::SendEventSystemMenu(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemMenuCalled(ev);
	}
}

INLINE void ISystem::SendEventSystemDataManager(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemDataManagerCalled(ev);
	}
}

INLINE void ISystem::SendEventHomeEnded(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemHomeEnded(ev);
	}
}

INLINE void ISystem::SendEventSleep(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemSleep(ev);
	}
}

INLINE void ISystem::SendEventLanguageChanged(const EventSystem *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventSystemListener *target = (*it);
		ASSERT_NULL(target);
		target->OnSystemLanguageChanged(ev);
	}
}


} // namespace
