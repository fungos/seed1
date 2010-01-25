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

GuiManager GuiManager::instance;

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
	return IModule::Reset();
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

INLINE int GuiManager::GetObjectType() const
{
	return Seed::ObjectGuiManager;
}

} // namespace
