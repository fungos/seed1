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

/*! \file IphTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#include <time.h>
#include <sys/time.h>

#define TAG "[Timer] "

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(Timer);

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
