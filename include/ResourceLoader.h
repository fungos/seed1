/*! \file ResourceLoader.h
	\author	Rafael Eduardo Gonchor
	\brief Defines the resource loader class interface
*/

#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "Mutex.h"
#include "Thread.h"

#include <vector>

namespace Seed {


class ResourceGroup;
class EventResourceLoader;
class IEventResourceLoaderListener;


/// Resource Loader
/**
Platform resource loader
*/
class ResourceLoader : public IModule, public IUpdatable, public Thread
{
	public:
		typedef std::vector<IEventResourceLoaderListener *> 	ListenerVector;
		typedef ListenerVector::iterator						ListenerIterator;

		typedef std::vector<ResourceGroup *> 					GroupVector;
		typedef GroupVector::iterator							GroupIterator;

	public:
		ResourceLoader();
		virtual ~ResourceLoader();

		//! Add a load request to the queue.
		/*!
			Add a load request to the queue.
		 */
		virtual void Add(ResourceGroup *group);

		//! Resource loader thread loop
		/*!
			Resource loader thread loop, if it returns false then it has finished.
		 */
		virtual BOOL Run(); // Thread

		/// Listeners pattern methods
		void AddListener(IEventResourceLoaderListener *listener);
		void RemoveListener(IEventResourceLoaderListener *listener);

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

	public:
		static ResourceLoader instance;

	protected:
		void SendEventGroupLoaded(const EventResourceLoader *ev);
		void SendEventQueueEmpty(const EventResourceLoader *ev);

	private:
		SEED_DISABLE_COPY(ResourceLoader);

	private:
		ListenerVector 	vListeners;
		GroupVector		vGroups;
		BOOL 			bRunning;
		Mutex			stMutex;
};


ResourceLoader *const pResourceLoader = &ResourceLoader::instance;


} // namespace


#endif // __RESOURCE_LOADER_H__

