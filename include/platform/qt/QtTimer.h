/*! \file QtTimer.h
	\author	Danny Angelo Carminati Grein
	\brief Timer QT Implementation
*/

#ifndef __QT_TIMER_H__
#define __QT_TIMER_H__

#ifdef _QT_

#include "interface/ITimer.h"
#include <QTime>


namespace Seed { namespace QT {


class Timer : public ITimer
{
	public:
		Timer();
		virtual ~Timer();

		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual u64 GetMilliseconds() const;
		virtual void Sleep(u32 ms) const;

	public:
		static Timer instance;
		QTime t;

	private:
		SEED_DISABLE_COPY(Timer);
};

Timer *const pTimer = &Timer::instance;


}} // namespace


#else // _QT_

	#error "Include 'Timer.h' instead 'platform/qt/QtTimer.h' directly."

#endif // _QT_
#endif // __QT_TIMER_H__
