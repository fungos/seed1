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

/*! \file SdlTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer SDL Implementation
*/

#if defined(_SDL_)

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#include <SDL/SDL.h>

#define TAG "[Timer] "

namespace Seed { namespace SDL {

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
