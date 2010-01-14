/*! \file SdlSystem.h
	\author	Danny Angelo Carminati Grein
	\brief System SDL Implementation
*/

#ifndef __SDL_SYSTEM_H__
#define __SDL_SYSTEM_H__

#ifdef _SDL_

#include "interface/ISystem.h"


namespace Seed { namespace SDL {


class System : public ISystem
{
	public:
		System();
		virtual ~System();

		// ISystem
		virtual void GoToMenu();
		virtual void Sleep();
		virtual void OnHomeCalled();
		virtual void WaitForRetrace(ISystem::eFrameRate rate);
		virtual void SetFrameRate(ISystem::eFrameRate rate);
		virtual void HangUp();
		virtual void GoToDataManager();

		virtual ISystem::eFrameRate GetFrameRate();

		virtual BOOL IsShuttingDown() const;
		virtual BOOL IsResetting() const;
		virtual BOOL IsSleeping() const;

		virtual void DisableHome();
		virtual void EnableHome();
		virtual BOOL IsHomeEnabled() const;
		virtual BOOL IsHomeRunning() const;
		virtual BOOL InitializeHome();

		virtual void EnableDefaultCursor(BOOL b);

		virtual const char *GetUsername() const;
		virtual const char *GetHomeFolder() const;
		virtual const char *GetApplicationDataFolder() const;
		virtual const char *GetSaveGameFolder() const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

		// IUpdatable
		virtual BOOL Update(f32 dt);

	public:
		static System instance;

	private:
		SEED_DISABLE_COPY(System);

	private:
		u32 		iRetraceCount;
		eFrameRate 	iFrameRate;
		BOOL 		bShutdown;
		BOOL		bSleeping;
		BOOL		bDefaultCursorEnabled;
		f32 		fElapsedTime;
		u64 		iLastFrameTime;
		u64 		iFpsTime;
};

System *const pSystem = &System::instance;


}} // namespace


#else // _SDL_

	#error "Include 'System.h' instead 'platform/sdl/SdlSystem.h' directly."

#endif // _SDL_
#endif // __SDL_SYSTEM_H__
