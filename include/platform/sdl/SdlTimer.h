/*! \file SdlTimer.h
	\author	Danny Angelo Carminati Grein
	\brief Timer SDL Implementation
*/

#ifndef __SDL_TIMER_H__
#define __SDL_TIMER_H__

#ifdef _SDL_

#include "interface/ITimer.h"
#include <time.h>


namespace Seed { namespace SDL {


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
		u64 fStart;


	private:
		Timer(const Timer &);
		Timer &operator=(const Timer &);
};

Timer *const pTimer = &Timer::instance;


}} // namespace


#else // _SDL_

	#error "Include 'Timer.h' instead 'platform/sdl/SdlTimer.h' directly."

#endif // _SDL_
#endif // __SDL_TIMER_H__
