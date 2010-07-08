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

/*! \file ISystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the System class interface
*/

#include "interface/ISystem.h"
#include "interface/IEventSystemListener.h"
#include "EventSystem.h"

namespace Seed {

const char *pcLanguageTable[] =
{
	"en_US",
	"pt_BR",
	"es_ES",
	"de_DE",
	"ja_JA",
	"cn_CN",
	"Unknown"
};

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

INLINE const char *ISystem::GetLanguageString() const
{
	return pcLanguageTable[this->nLanguage];
}

INLINE const char *ISystem::GetUsername() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE const char *ISystem::GetHomeFolder() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE const char *ISystem::GetApplicationDataFolder() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE const char *ISystem::GetSaveGameFolder() const
{
	SEED_ABSTRACT_METHOD;
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

INLINE BOOL ISystem::IsRequired() const
{
	return TRUE;
}

INLINE const char *ISystem::GetObjectName() const
{
	return "System";
}

} // namespace
