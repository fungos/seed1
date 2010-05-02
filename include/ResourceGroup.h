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

/*! \file ResourceGroup.h
	\author	Rafael Eduardo Gonchor
	\brief Defines the resource group class
*/

#ifndef __RESOURCEGROUP_H__
#define __RESOURCEGROUP_H__

#include "Log.h"
#include "interface/IEvent.h"
#include "interface/IEventListener.h"

#include "MemoryManager.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "SeedInit.h"

#include <vector>
#include <algorithm>

namespace Seed {

class SEED_CORE_API ResourceGroup
{
	friend class ResourceLoader;

	public:
		ResourceGroup();
		virtual ~ResourceGroup();

		void Add(const char *filename, Seed::eObjectType resourceType = Seed::ObjectSprite, IMemoryPool *pool = pDefaultPool, ResourceManager *res = pResourceManager);

	protected:
		typedef struct SEED_CORE_API QueueItem
		{
			const char 			*filename;
			IResource			*resource;
			Seed::eObjectType 	resourceType;
			IMemoryPool			*pool;
			ResourceManager		*resManager;
			u32					startTime;
			BOOL				erased;
		} QueueItem;

		typedef std::vector<QueueItem *> QueueVector;
		typedef QueueVector::iterator	QueueIterator;

	protected:
		BOOL Load();
		BOOL Unload();

		void SetLoaded();
		BOOL IsLoaded() const;

	protected:
		QueueVector		queue;
		BOOL			bLoaded;

	private:
		SEED_DISABLE_COPY(ResourceGroup);
};

} // namespace

#endif // __RESOURCEGROUP_H__
