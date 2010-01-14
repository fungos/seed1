/*! \file ISystem.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the System class interface
*/

#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__


#include "Log.h"
#include "IModule.h"
#include "IUpdatable.h"
#include "Enum.h"
#include "IEvent.h"
#include "IEventListener.h"

#include <vector>
#include <algorithm>

#define SYSTEM_RETRACE_HISTORY_MAX	64

namespace Seed {


class EventSystem;
class IRenderer;
class IEventSystemListener;


/// System Interface
/**
Platform system interface.
*/
class ISystem : public IModule, public IUpdatable
{
	typedef std::vector<IEventSystemListener *> 	ListenerVector;
	typedef ListenerVector::iterator			  	ListenerIterator;

	public:
		//! Basic frame rate values.
		/*!
			Some platform may not support nativelly some frame rates values.
		 */
		enum eFrameRate
		{
			RATE_60FPS = 60, /*!< 60 Frames per second. */
			RATE_30FPS = 30, /*!< 30 Frames per second. */
			RATE_20FPS = 20, /*!< 20 Frames per second. */
			RATE_15FPS = 15, /*!< 15 Frames per second. */
			RATE_UNLIMITED = 10000 /*!< 100 or more frames per second. */
		};

		//! System event type
		/*!
			Events for horizontal blank interrupt and vertical blank interrupt.
		 */
		enum eEvent
		{
			EVENT_HBLANK = 0, /*!< Horizontal blank interrupt event ocurred */
			EVENT_VBLANK = 1  /*!< Vertical blank interrupt event ocurred */
		};

	public:
		ISystem();
		virtual ~ISystem();

		//! Goes to menu if available.
		/*!
			Unloads application and goes back to SO on platforms that supports it. If the platform doesn't
			support, you can implement it anyway you wan't or keep it empty.
		 */
		virtual void GoToMenu() = 0;

		//! Disables calls to the home menu.
		/*!
			Disables calls to menu, this is usually done to avoid calls to it while a loading is in progress.
		 */
		virtual void DisableHome() = 0;

		//! Enables calls to the home menu.
		/*!
			Calls to menu are enabled by default.
		 */
		virtual void EnableHome() = 0;

		//! Returns if home calls are enabled.
		/*!
			Returns if home calls are enabled or not.
		 */
		virtual BOOL IsHomeEnabled() const = 0;

		//! Returns if home is currently running.
		/*!
			Returns if home is running or not.
		 */
		virtual BOOL IsHomeRunning() const = 0;

		//! Initializes the home button class.
		/*!
			Initializes the home button class.
		 */
		virtual BOOL InitializeHome() = 0;

		//! Goes to the system data manager if available.
		/*!
			Unloads application and goes back to SO on platforms that supports it. If the platform doesn't
			support, you can implement it anyway you wan't or keep it empty.
		 */
		virtual void GoToDataManager() = 0;

		//! Performs a system lockup.
		/*!
			When a fatal error occurs and not even a reset is guaranteed, the system should be put on a
			locked state and wait for a hardware reset.
		 */
		virtual void HangUp() = 0;

		//! Check shutdown.
		/*!
			Checks whatever system is shutting down.

			\return TRUE if system is shutting down, FALSE otherwise.
		 */
		virtual BOOL IsShuttingDown() const = 0;

		//! Check reset.
		/*!
			Checks whatever system is resetting.

			\return TRUE if system is resetting, FALSE otherwise.
		 */
		virtual BOOL IsResetting() const = 0;

		//! Activate sleep/standby mode.
		/*!
			Activate sleep/standby freature on platforms that supports it. If the platform doesn't
			support, you can implement it anyway you wan't or keep it empty.
		 */
		virtual void Sleep() = 0;

		//! Check sleep/standby mode.
		/*!
			Checks whatever system is in sleep/standby mode.

			\return TRUE if system is in sleep/standby mode, FALSE otherwise.
		 */
		virtual BOOL IsSleeping() const = 0;

		//! Goes to home interface if available.
		/*!
			Executes platform specific code to manage those platforms that have a home button interface
			such as Wii and PSP, if the platform doesn't have this feature, keep it empty.
		 */
		virtual void OnHomeCalled() = 0;

		//! Wait for retrace.
		/*!
			\param frameRate Retrace rate.
		 */
		virtual void WaitForRetrace(eFrameRate frameRate) = 0;

		//! Sets the desired frame rate.
		/*!
			Sets the desired frame rate for running the entire system.
			\param frameRate Frame rate of the system
		 */
		virtual void SetFrameRate(ISystem::eFrameRate frameRate) = 0;

		//! Gets the desired frame rate.
		/*!
			Returns the selected frame rate.
			\returns a ISystem::eFrameRate enumeration;
		 */
		virtual ISystem::eFrameRate GetFrameRate() = 0;

		//! Get current user name
		/*!
			Get current user name.
			\returns a string containing the user name.
		*/
		virtual const char *GetUsername() const;

		//! Get user home folder
		/*!
			Get current user home folder.
			\returns a string containing the user home folder.
		*/
		virtual const char *GetHomeFolder() const;

		//! Get user application data folder
		/*!
			Where the user application data must be written
			\returns a string containing the user appdata folder.
		*/
		virtual const char *GetApplicationDataFolder() const;

		//! Get save game folder
		/*!
			Where the save game must be written
			\returns a string containing the save game path.
		*/
		virtual const char *GetSaveGameFolder() const;

		//! Enable/Disable default system cursor.
		/*!
			Enable/Disable default system cursor.
			\param b Enable?
		 */
		virtual void EnableDefaultCursor(BOOL b);

		//! Returns if home calls are enabled.
		/*!
			Returns if home calls are enabled or not.
		 */
		virtual BOOL IsDefaultCursorEnabled() const;

		//! Sets the application title.
		/*!
			Sets the application title, mainly used for savegame data.
			\param title Title of the application;
		 */
		void SetApplicationTitle(const char *title);

		//! Returns the application title.
		/*!
			Returns the application title.
			\returns a const char * string;
		 */
		const char *GetApplicationTitle() const;

		//! Sets the application description.
		/*!
			Sets the application description, mainly used for savegame data.
			\param descr Description of the application ;
		 */
		void SetApplicationDescription(const char *descr);

		//! Returns the application description.
		/*!
			Returns the application description.
			\returns a const char * string;
		 */
		const char *GetApplicationDescription() const;

		//! Set the system language.
		/*!
			Set/changes the system current language.
		 */
		void SetLanguage(Seed::eLanguage lang);

		//! Get current system language.
		/*!
			Get current system language.
			\returns the language being used.
		 */
		Seed::eLanguage GetLanguage() const;

		//! Get current system language.
		/*!
			Get current system language.
			\returns a string representing the language being used.
		 */
		const char *GetLanguageString() const;

		/// Listeners pattern methods
		void AddListener(IEventSystemListener *listener);
		void RemoveListener(IEventSystemListener *listener);

	protected:
		void SendEventReset(const EventSystem *ev);
		void SendEventShutdown(const EventSystem *ev);
		void SendEventSystemMenu(const EventSystem *ev);
		void SendEventSystemDataManager(const EventSystem *ev);
		void SendEventHomeEnded(const EventSystem *ev);
		void SendEventSleep(const EventSystem *ev);
		void SendEventLanguageChanged(const EventSystem *ev);

	protected:
		ListenerVector vListeners;

		const char		*pStrAppName;
		const char		*pStrAppDescription;
		Seed::eLanguage nLanguage;

		BOOL			bDefaultCursorEnabled;
		u32				iRetraceIndex;
		u32				arRetraceCount[SYSTEM_RETRACE_HISTORY_MAX];

	private:
		SEED_DISABLE_COPY(ISystem);
};


} // namespace


#endif // __ISYSTEM_H__

