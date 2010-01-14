/*! \file IphTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer Iphone Implementation
*/

#ifdef _IPHONE_

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#include <time.h>
#include <sys/time.h>

#define TAG "[Timer] "


namespace Seed { namespace iPhone {


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
	struct timeval tv;
	gettimeofday(&tv, 0);
	//fStart = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	fStart = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000000ULL);

	return TRUE;
}


INLINE BOOL Timer::Shutdown()
{
	return this->Reset();
}


INLINE const u64 Timer::GetMilliseconds() const
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	//u64 ret = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	//return (ret - fStart) / 1000;
	u64 ret = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000000ULL);
	return (ret - fStart);
}

	
INLINE void Timer::Sleep(u32 ms) const
{
}
	

}} // namespace

#endif // _IPHONE_
