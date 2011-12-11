/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file PS3Timer.cpp
	\author	Danny Angelo Carminati Grein
	\brief ps3dev timer implementation
*/

#if defined(_PS3DEV_)

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#include <sys/systime.h>

#define TAG "[Timer] "

namespace Seed { namespace PS3 {

SEED_SINGLETON_DEFINE(Timer);

Timer::Timer()
	: fStart(0)
	, fClock(0)
{
}

Timer::~Timer()
{
}

INLINE BOOL Timer::Initialize()
{
	Log(TAG "Initializing...");

	fClock = sysGetTimebaseFrequency();
	this->Reset();

	Log(TAG "Initialization completed.");

	return TRUE;
}

INLINE BOOL Timer::Reset()
{
	u64 sec = 0;
	u64 nsec = 0;
	sysGetCurrentTime(&sec, &nsec);

	fStart = (sec * 1000ULL) + ((nsec / 1000ULL) / 1000ULL);

	return TRUE;
}

INLINE BOOL Timer::Shutdown()
{
	return this->Reset();
}

INLINE u64 Timer::GetMilliseconds() const
{
	u64 sec = 0;
	u64 ns = 0;
	sysGetCurrentTime(&sec, &ns);

	u64 us = ns / 1000ULL;
	u64 ms = us / 1000ULL;
	u64 x =  sec * 1000ULL;
	u64 ret = x + ms - fStart;

	return ret;
}

INLINE void Timer::Sleep(u32 ms) const
{
	sysUsleep(ms * 1000);
}

}} // namespace

#endif // _PS3DEV_
