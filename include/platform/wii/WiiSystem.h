/*! \file WiiSystem.h
	\author	Danny Angelo Carminati Grein
	\brief System init, VBlank and Interupt functionality
*/

#ifndef __WII_SYSTEM_H__
#define __WII_SYSTEM_H__

#ifdef _WII_

#include "interface/ISystem.h"

namespace Seed { namespace WII {

class System : public ISystem
{
	public:
		System();
		virtual ~System();

		virtual void OnHomeCalled();
		virtual void GoToMenu();
		virtual void GoToDataManager();
		virtual void HangUp();

		virtual BOOL IsShuttingDown() const;
		virtual BOOL IsResetting() const;

		virtual void Sleep();
		virtual BOOL IsSleeping() const;

		virtual void WaitForRetrace(ISystem::eFrameRate rate);
		virtual void SetFrameRate(ISystem::eFrameRate rate);
		virtual ISystem::eFrameRate GetFrameRate();

		virtual void DisableHome();
		virtual void EnableHome();

		virtual BOOL IsHomeEnabled() const;
		virtual BOOL IsHomeRunning() const;

		virtual BOOL InitializeHome();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

		// IUpdatable
		virtual BOOL Update(f32 delta);

	public:
		static System instance;

	private:
		SEED_DISABLE_COPY(System);

		static void System::ResetCallback();
		static void System::PowerCallback();

	private:
		u32 retraceCount;
		eFrameRate frameRate;
		static BOOL g_Reset;
		static BOOL g_Power;
};

System *const pSystem = &System::instance;

}} // namespace

#else // _WII_
	#error "Include 'System.h' instead 'platform/wii/WiiSystem.h' directly."
#endif // _WII_
#endif // __WII_SYSTEM_H__
