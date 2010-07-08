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

/*! \file GuiManager.h
	\author Danny Angelo Carminati Grein
	\brief GUI Manager
*/

#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include "Defines.h"
#include "interface/IWidget.h"
#include "interface/IObject.h"
#include "interface/IModule.h"
#include "interface/IInputPointer.h"
#include "interface/IEventInputPointerListener.h"
#include "WidgetContainer.h"
#include "Singleton.h"

namespace Seed {

class SEED_CORE_API GuiManager : public IModule, public IUpdatable, public IEventInputPointerListener
{
	SEED_SINGLETON_DECLARE(GuiManager);
	public:
		void Add(IWidget *widget);
		void Remove(IWidget *widget);

		void SetFocus(IWidget *widget);
		IWidget *GetFocused() const;

		// IEventInputPointerListener
		virtual void OnInputPointerPress(const EventInputPointer *ev);
		virtual void OnInputPointerRelease(const EventInputPointer *ev);
		virtual void OnInputPointerMove(const EventInputPointer *ev);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IObject
		virtual const char *GetObjectName() const;

	private:
		SEED_DISABLE_COPY(GuiManager);

	private:
		IWidget *pFocused;
		WidgetContainer cContainer;
};

#define pGuiManager GuiManager::GetInstance()

} // namespace

#endif // __GUI_MANAGER_H__
