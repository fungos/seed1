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


ResourceLoader ResourceLoader::instance;


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


} // namespace
