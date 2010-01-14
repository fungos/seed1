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


namespace Seed {


class GuiManager : public IModule, public IUpdatable, public IEventInputPointerListener
{
	public:
		static GuiManager instance;

	public:
		GuiManager();
		virtual ~GuiManager();

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
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(GuiManager);

	private:
		IWidget *pFocused;
		WidgetContainer cContainer;
};

GuiManager *const pGuiManager = &GuiManager::instance;


} // namespace


#endif // __GUI_MANAGER_H__

