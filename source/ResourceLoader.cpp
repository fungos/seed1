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

/*! \file ResourceLoader.cpp
	\author	Rafael Eduardo Gonchor
	\brief Defines the resource loader class
*/

#include "Log.h"
#include "ResourceLoader.h"
#include "ResourceGroup.h"
#include "EventResourceLoader.h"
#include "interface/IEventResourceLoaderListener.h"
#include <algorithm>

#define TAG		"[ResourceLoader] "

namespace Seed {

SEED_SINGLETON_DEFINE(ResourceLoader);

ResourceLoader::ResourceLoader()
	: vListeners()
	, vGroups()
	, bRunning(FALSE)
	, stMutex()
{
}

ResourceLoader::~ResourceLoader()
{
}

INLINE BOOL ResourceLoader::Reset()
{
	IModule::Reset();
	return TRUE;
}

INLINE BOOL ResourceLoader::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	this->Create();
	this->bRunning = TRUE;
	Log(TAG "Initialization completed.");

	return TRUE;
}

INLINE BOOL ResourceLoader::Shutdown()
{
	this->bRunning = FALSE;
	IModule::Shutdown();
	Log(TAG "Terminated.");

	return TRUE;
}

BOOL ResourceLoader::Update(f32 dt)
{
	UNUSED(dt);

	if (!bRunning)
		return FALSE;

	ResourceGroup *group = NULL;

	stMutex.Lock();
	if (vGroups.size() > 0)
	{
		group = (*vGroups.begin());
	}
	stMutex.Unlock();

	if (!group)
		return TRUE;

	stMutex.Lock();
	if (group->IsLoaded())
	{
		EventResourceLoader ev;
		ev.SetGroup(group);
		SendEventGroupLoaded(&ev);

		group->Unload();

		GroupIterator p = std::find(vGroups.begin(), vGroups.end(), group);

		if (p != vGroups.end())
			vGroups.erase(p);

		if (vGroups.size() == 0)
		{
			vGroups.clear();
			std::vector<ResourceGroup *>().swap(vGroups);
		}

		//glResourceManager.Print();
	}
	stMutex.Unlock();

	return TRUE;
}

BOOL ResourceLoader::Run()
{
	BOOL ret = Thread::Run();
	//Log("Load thread loop");
	if (ret)
	{
		ResourceGroup *group = NULL;

		stMutex.Lock();
		if (vGroups.size() > 0)
		{
			group = (*vGroups.begin());
		}
		stMutex.Unlock();

		if (!group)
			return TRUE;

		if (group->IsLoaded())
			return TRUE;

		if (group->Load())
		{
			stMutex.Lock();
			group->SetLoaded();
			stMutex.Unlock();
		}

		ret = this->bRunning;
	}

	pTimer->Sleep(10);
	return ret;
}

INLINE void ResourceLoader::Add(ResourceGroup *group)
{
	stMutex.Lock();
	vGroups.push_back(group);
	stMutex.Unlock();
}

INLINE void ResourceLoader::AddListener(IEventResourceLoaderListener *listener)
{
	ASSERT_NULL(listener);
	vListeners.push_back(listener);
}

INLINE void ResourceLoader::RemoveListener(IEventResourceLoaderListener *listener)
{
	ASSERT_NULL(listener);
	ListenerIterator p = std::find(vListeners.begin(), vListeners.end(), listener);

	if (p != vListeners.end())
		vListeners.erase(p);

	if (!vListeners.size())
	{
		vListeners.clear();
		ListenerVector().swap(vListeners);
	}
}

INLINE void ResourceLoader::SendEventGroupLoaded(const EventResourceLoader *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventResourceLoaderListener *target = (*it);
		ASSERT_NULL(target);
		target->OnGroupLoaded(ev);
	}
}

INLINE void ResourceLoader::SendEventQueueEmpty(const EventResourceLoader *ev)
{
	ASSERT_NULL(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventResourceLoaderListener *target = (*it);
		ASSERT_NULL(target);
		target->OnQueueEmpty(ev);
	}
}

INLINE const char *ResourceLoader::GetObjectName() const
{
	return "ResourceLoader";
}

} // namespace
