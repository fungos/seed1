/*! \file WiiTimer.h
	\author	Danny Angelo Carminati Grein
	\brief Timer
*/


#ifndef __WII_TIMER_H__
#define __WII_TIMER_H__


#if defined(_WII_)


#include "interface/ITimer.h"


namespace Seed { namespace WII {


class Timer : public ITimer
{
	public:
		Timer();
		virtual ~Timer();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual const u64 GetMilliseconds() const;
		virtual void Sleep(u32 ms) const;


	public:
		static Timer instance;
		u64 start;


	private:
		SEED_DISABLE_COPY(Timer);
};


Timer *const pTimer = &Timer::instance;


}} // namespace


#else // _WII_

	#error "Include 'Timer.h' instead 'platform/wii/WiiTimer.h' directly."

#endif // _WII_
#endif // __WII_TIMER_H__
