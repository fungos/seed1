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

/*! \file QtTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer QT Implementation
*/

#if defined(_QT_)

#include <QWaitCondition>

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#define TAG "[Timer] "

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(Timer);

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
