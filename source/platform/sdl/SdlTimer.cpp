/*! \file SdlTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer SDL Implementation
*/

#ifdef _SDL_

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#include <SDL/SDL.h>

#define TAG "[Timer] "


namespace Seed { namespace SDL {


Timer Timer::instance;


Timer::Timer()
	: fStart(0)
{
}


Timer::~Timer()
{
}


INLINE BOOL Timer::Initialize()
{
	Log(TAG "Initializing...");
	this->Reset();
	Log(TAG "Initialization completed.");

	return TRUE;
}


INLINE BOOL Timer::Reset()
{
	fStart = SDL_GetTicks();

	return TRUE;
}


INLINE BOOL Timer::Shutdown()
{
	return this->Reset();
}


INLINE u64 Timer::GetMilliseconds() const
{
	u64 ret = SDL_GetTicks();

	return (ret - fStart);
}


INLINE void Timer::Sleep(u32 ms) const
{
	SDL_Delay(ms);
}


}} // namespace


#endif // _SDL_

