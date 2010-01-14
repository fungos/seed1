/*! \file IphSystem.h
	\author	Danny Angelo Carminati Grein
	\brief System Iphone Implementation
*/

#ifndef __IPH_SYSTEM_H__
#define __IPH_SYSTEM_H__

#ifdef _IPHONE_

#include "interface/ISystem.h"


namespace Seed { namespace iPhone {


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
		virtual void DisableHome();
		virtual void EnableHome();

		virtual ISystem::eFrameRate GetFrameRate();

		virtual BOOL IsShuttingDown() const;
		virtual BOOL IsResetting() const;
		virtual BOOL IsSleeping() const;
		virtual BOOL IsHomeEnabled() const;
		virtual BOOL IsHomeRunning() const;
		virtual BOOL InitializeHome();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

		// IUpdatable
		virtual BOOL Update(f32 dt);


	public:
		static System instance;


	private:
		System(const System &);
		System &operator=(const System &);


	private:
		u32 iRetraceCount;
		eFrameRate iFrameRate;
};

System *const pSystem = &System::instance;


}} // namespace


#else // _IPHONE_

	#error "Include 'System.h' instead 'platform/iphone/IphSystem.h' directly."

#endif // _IPHONE_
#endif // __IPH_SYSTEM_H__
