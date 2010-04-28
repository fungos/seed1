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

/*! \file IEventSystemListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event system listener class interface
*/

#ifndef __IEVENTSYSTEMLISTENER_H__
#define __IEVENTSYSTEMLISTENER_H__

#include "IEventListener.h"
#include "Defines.h"

namespace Seed {

class EventSystem;

class SEED_CORE_API IEventSystemListener : public IEventListener
{
	public:
		IEventSystemListener();
		virtual ~IEventSystemListener();

		virtual void OnSystemReset(const EventSystem *ev);
		virtual void OnSystemShutdown(const EventSystem *ev);
		virtual void OnSystemMenuCalled(const EventSystem *ev);
		virtual void OnSystemDataManagerCalled(const EventSystem *ev);
		virtual void OnSystemHomeEnded(const EventSystem *ev);
		virtual void OnSystemSleep(const EventSystem *ev);
		virtual void OnSystemLanguageChanged(const EventSystem *ev);

	private:
		SEED_DISABLE_COPY(IEventSystemListener);
};

} // namespace

#endif // __IEVENTSYSTEMLISTENER_H__
