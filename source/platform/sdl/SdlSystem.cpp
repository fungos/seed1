/*! \file SdlSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief System SDL Implementation
*/

#ifdef _SDL_

#include "Defines.h"
#include "System.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Timer.h"
#include "EventSystem.h"

#include "platform/pc/platform.h"

#define TAG "[System] "

#define MAX_FRAMESKIP_THRESHOLD 10

namespace Seed { namespace SDL {


System System::instance;


System::System()
	: iRetraceCount(0)
	, iFrameRate(RATE_60FPS)
	, bShutdown(FALSE)
	, bSleeping(FALSE)
	, bDefaultCursorEnabled(FALSE)
	, fElapsedTime(0.0f)
	, iLastFrameTime(0)
	, iFpsTime(0)
{
}

System::~System()
{
}

BOOL System::Reset()
{
	return TRUE;
}

BOOL System::Initialize()
{
	Log(TAG "Initializing...");
	print_system_info();

	if (SDL_Init(SDL_INIT_TIMER) < 0 ) // SDL_INIT_VIDEO
	{
		Info(TAG "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	//atexit(SDL_Quit);
	//freopen("CON", "w", stdout);
	//freopen("CON", "w", stderr);

	Log(TAG "Initialization completed.");

	return TRUE;
}

BOOL System::Shutdown()
{
	this->bShutdown = TRUE;
	Log(TAG "Terminating...");

	return TRUE;
}

BOOL System::Update(f32 dt)
{
	UNUSED(dt);

	u8 state = SDL_GetAppState();
	if ((state & SDL_APPACTIVE) != SDL_APPACTIVE || (state & SDL_APPINPUTFOCUS) != SDL_APPINPUTFOCUS)
	{
		if (!this->bSleeping)
		{
			this->bSleeping = TRUE;

			EventSystem ev;
			this->SendEventSleep(&ev);
		}
	}
	else
	{
		if (this->bSleeping)
		{
			this->bSleeping = FALSE;

			EventSystem ev;
			this->SendEventSleep(&ev);
		}
	}

	//this->WaitForRetrace(this->iFrameRate);
	return TRUE;
}

INLINE void System::Sleep()
{
	Log(TAG "WARNING: Platform doesnt support sleep mode.");
}

INLINE BOOL System::IsSleeping() const
{
	return this->bSleeping;
}

INLINE BOOL System::IsShuttingDown() const
{
	return this->bShutdown;
}

INLINE BOOL System::IsResetting() const
{
	return FALSE;
}

INLINE void System::WaitForRetrace(eFrameRate rate)
{
	++this->iRetraceCount;

	if (!this->iLastFrameTime)
		this->iLastFrameTime = pTimer->GetMilliseconds();

	f32 fFrameMaxTime			= 1000.0f / (f32)rate;

	do
	{
		//hold fps
		u64 iTime				= pTimer->GetMilliseconds();
		u64 iFrameTime			= iTime - iLastFrameTime;
		this->iFpsTime			+= iFrameTime;
		this->fElapsedTime		+= iFrameTime;
		this->iLastFrameTime	= iTime;
	} while (this->fElapsedTime < fFrameMaxTime);

	this->fElapsedTime -= fFrameMaxTime;

	//Raptor: test fix for when WM_PAINT stops comming for a long time due to the
	//user moving the window, for instance. Tries to avoid the retrace trying to
	//catch up with the lost frame time
	if ((this->fElapsedTime / fFrameMaxTime) > MAX_FRAMESKIP_THRESHOLD)
		this->fElapsedTime = 0;

	if (this->iFpsTime > 1000)
	{
		Dbg("FPS: %d", this->iRetraceCount);

		arRetraceCount[iRetraceIndex++] = this->iRetraceCount;
		if (iRetraceIndex >= SYSTEM_RETRACE_HISTORY_MAX)
			iRetraceIndex = 0;

		this->iFpsTime -= 1000;
		this->iRetraceCount = 0;
	}
}

INLINE void System::SetFrameRate(eFrameRate rate)
{
	this->iFrameRate = rate;
}

INLINE ISystem::eFrameRate System::GetFrameRate()
{
	return this->iFrameRate;
}

INLINE const char *System::GetUsername() const
{
	return get_user_name();
}

INLINE const char *System::GetHomeFolder() const
{
	return get_user_home_folder();
}

INLINE const char *System::GetApplicationDataFolder() const
{
	return get_user_appdata_folder();
}

INLINE const char *System::GetSaveGameFolder() const
{
	return get_user_savegame_folder();
}

INLINE void System::GoToMenu()
{
}

INLINE void System::OnHomeCalled()
{
}

INLINE void System::GoToDataManager()
{
}

INLINE void System::HangUp()
{
}

INLINE BOOL System::IsHomeEnabled() const
{
	return FALSE;
}

INLINE BOOL System::IsHomeRunning() const
{
	return FALSE;
}

INLINE BOOL System::InitializeHome()
{
	return TRUE;
}

INLINE void System::EnableHome()
{
}

INLINE void System::DisableHome()
{
}

INLINE void System::EnableDefaultCursor(BOOL b)
{
	ISystem::EnableDefaultCursor(b);

	SDL_ShowCursor(b);
}

}} // namespace


#endif // _SDL_
