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

/*! \file IEventFileSystemListener.h
	\author	Rafael Eduardo Gonchor
	\brief Defines the event file system listener class interface
*/

#ifndef __IEVENTFILESYSTEMLISTENER_H__
#define __IEVENTFILESYSTEMLISTENER_H__

#include "IEventListener.h"
#include "Defines.h"

namespace Seed {

class EventFileSystem;

class SEED_CORE_API IEventFileSystemListener : public IEventListener
{
	public:
		IEventFileSystemListener();
		virtual ~IEventFileSystemListener();

		virtual void OnFileSystemFatalError(const EventFileSystem *ev);
		virtual void OnFileSystemWrongDiskError(const EventFileSystem *ev);
		virtual void OnFileSystemNoDiskError(const EventFileSystem *ev);
		virtual void OnFileSystemRetryError(const EventFileSystem *ev);
		virtual void OnFileSystemOperationResumed(const EventFileSystem *ev);
		virtual void OnFileSystemLoadCompleted(const EventFileSystem *ev);

	private:
		SEED_DISABLE_COPY(IEventFileSystemListener);
};

} // namespace

#endif // __IEVENTFILESYSTEMLISTENER_H__
