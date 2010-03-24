/*! \file IphTimer.h
	\author	Danny Angelo Carminati Grein
	\brief Timer Iphone Implementation
*/

#ifndef __IPH_TIMER_H__
#define __IPH_TIMER_H__

#ifdef _IPHONE_

#include "interface/ITimer.h"
#include <time.h>

namespace Seed { namespace iPhone {

class Timer : public ITimer
{
	public:
		Timer();
		virtual ~Timer();

		// ITimer
		virtual const u64 GetMilliseconds() const;
		virtual void Sleep(u32 ms) const;

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static Timer instance;
		u64 fStart;

	private:
		SEED_DISABLE_COPY(Timer);
};

Timer *const pTimer = &Timer::instance;

}} // namespace

#else // _IPHONE_
	#error "Include 'Timer.h' instead 'platform/iphone/IphTimer.h' directly."
#endif // _IPHONE_
#endif // __IPH_TIMER_H__
