/*! \file QtSystem.h
	\author	Danny Angelo Carminati Grein
	\brief System QT Implementation
*/

#ifndef __QT_SYSTEM_H__
#define __QT_SYSTEM_H__

#ifdef _QT_

#include <QWidget>
#include "interface/ISystem.h"


namespace Seed { namespace QT {


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

		virtual void WriteOut(const char *msg);
		virtual void WriteErr(const char *msg);
		virtual void SetWidget(QWidget *widget);

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Shutdown();
		virtual BOOL Reset();

	public:
		static System instance;

	private:
		SEED_DISABLE_COPY(System);

	private:
		u32 		iRetraceCount;
		eFrameRate 	iFrameRate;
		BOOL 		bShutdown;
		f32 		fElapsedTime;
		u64 		iLastFrameTime;
		u64 		iFpsTime;

		QWidget		*pWidget;
};

System *const pSystem = &System::instance;


}} // namespace


#else // _QT_

	#error "Include 'System.h' instead 'platform/qt/QtSystem.h' directly."

#endif // _QT_
#endif // __QT_SYSTEM_H__
