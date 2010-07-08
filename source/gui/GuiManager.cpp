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

/*! \file GuiManager.cpp
	\author Danny Angelo Carminati Grein
	\brief GUI Manager
*/

#include "MemoryManager.h"
#include "Input.h"
#include "Screen.h"
#include "Log.h"
#include "gui/GuiManager.h"
#include "Enum.h"

namespace Seed {

SEED_SINGLETON_DEFINE(GuiManager);

GuiManager::GuiManager()
	: pFocused(NULL)
	, cContainer()
	//, vWidget()
{
	//this->Reset();
}

GuiManager::~GuiManager()
{
	this->Reset();
}

INLINE BOOL GuiManager::Initialize()
{
	IModule::Initialize();

	this->Reset();

	pInput->AddPointerListener(this);
	cContainer.SetWidth(1.0f);
	cContainer.SetHeight(1.0f);

	return TRUE;
}

INLINE BOOL GuiManager::Reset()
{
	this->pFocused = NULL;
	return TRUE; // abstract IModule::Reset();
}

INLINE BOOL GuiManager::Shutdown()
{
	pInput->RemovePointerListener(this);
	return IModule::Shutdown();;
}

INLINE BOOL GuiManager::Update(f32 dt)
{
	cContainer.Update(dt);
	return TRUE;
}

INLINE void GuiManager::SetFocus(IWidget *widget)
{
	this->pFocused = widget;
}

INLINE IWidget *GuiManager::GetFocused() const
{
	return this->pFocused;
}

INLINE void GuiManager::Add(IWidget *widget)
{
	cContainer.Add(widget);
}

INLINE void GuiManager::Remove(IWidget *widget)
{
	cContainer.Remove(widget);
}

INLINE void GuiManager::OnInputPointerPress(const EventInputPointer *ev)
{
	cContainer.OnInputPointerPress(ev);
}

INLINE void GuiManager::OnInputPointerRelease(const EventInputPointer *ev)
{
	cContainer.OnInputPointerRelease(ev);
}

INLINE void GuiManager::OnInputPointerMove(const EventInputPointer *ev)
{
	cContainer.OnInputPointerMove(ev);
}

INLINE const char *GuiManager::GetObjectName() const
{
	return "GuiManager";
}

} // namespace
