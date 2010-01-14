/*! \file QtTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer QT Implementation
*/

#ifdef _QT_

#include <QWaitCondition>

#include "Defines.h"
#include "Timer.h"
#include "Log.h"


#define TAG "[Timer] "


namespace Seed { namespace QT {


Timer Timer::instance;


Timer::Timer()
{
}

Timer::~Timer()
{
}

INLINE BOOL Timer::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	this->Reset();
	Log(TAG "Initialization completed.");

	return TRUE;
}

INLINE BOOL Timer::Reset()
{
	IModule::Reset();
	t.start();

	return TRUE;
}

INLINE BOOL Timer::Shutdown()
{
	this->Reset();
	IModule::Shutdown();

	return TRUE;
}

INLINE u64 Timer::GetMilliseconds() const
{
	return static_cast<u64>(t.elapsed());
}

INLINE void Timer::Sleep(u32 ms) const
{
	UNUSED(ms);
	//QWaitCondition sleep;
	//sleep.wait(ms);
}


}} // namespace


#endif // _QT_

