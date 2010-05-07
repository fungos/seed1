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
#include "Singleton.h"

#include <vector>

namespace Seed {

class ResourceGroup;
class EventResourceLoader;
class IEventResourceLoaderListener;

/// Resource Loader
/**
Platform resource loader
*/
class SEED_CORE_API ResourceLoader : public IModule, public IUpdatable, public Thread
{
	SEED_SINGLETON_DECLARE(ResourceLoader);

	public:
		typedef std::vector<IEventResourceLoaderListener *> 	ListenerVector;
		typedef ListenerVector::iterator						ListenerIterator;

		typedef std::vector<ResourceGroup *> 					GroupVector;
		typedef GroupVector::iterator							GroupIterator;

	public:
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

		// IObject
		virtual const char *GetObjectName() const;

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

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(ResourceLoader);
}

} // namespace

#endif // __RESOURCE_LOADER_H__
