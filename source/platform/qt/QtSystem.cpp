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

/*! \file QtSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief System QT Implementation
*/

#if defined(_QT_)

#include "Defines.h"
#include "System.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Timer.h"

#define TAG "[System] "

#define MAX_FRAMESKIP_THRESHOLD 10

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(System);

System::System()
	: iRetraceCount(0)
	, iFrameRate(RATE_60FPS)
	, bShutdown(FALSE)
	, fElapsedTime(0.0f)
	, iLastFrameTime(0)
	, iFpsTime(0)
{
}

System::~System()
{
}

BOOL System::Reset()
{
	IModule::Reset();
	return TRUE;
}

BOOL System::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	Log(TAG "Initialization completed.");

	return TRUE;
}

BOOL System::Shutdown()
{
	this->bShutdown = TRUE;
	IModule::Shutdown();
	Log(TAG "Terminating...");

	return TRUE;
}

BOOL System::Update(f32 dt)
{
	UNUSED(dt);
	//this->WaitForRetrace(this->iFrameRate);
	return TRUE;
}

INLINE void System::Sleep()
{
}

INLINE BOOL System::IsSleeping() const
{
	return FALSE;
}

INLINE BOOL System::IsShuttingDown() const
{
	return this->bShutdown;
}

INLINE BOOL System::IsResetting() const
{
	return FALSE;
}

INLINE void System::WaitForRetrace(eFrameRate rate)
{
	++this->iRetraceCount;

	if (!this->iLastFrameTime)
		this->iLastFrameTime = pTimer->GetMilliseconds();

	f32 fFrameMaxTime			= 1000.0f / (f32)rate;

	do
	{
		//hold fps
		u64 iTime				= pTimer->GetMilliseconds();
		u64 iFrameTime			= iTime - iLastFrameTime;
		this->iFpsTime			+= iFrameTime;
		this->fElapsedTime		+= iFrameTime;
		this->iLastFrameTime	= iTime;
	} while (this->fElapsedTime < fFrameMaxTime);

	Dbg("Loop. %d", this->iFpsTime);

	this->fElapsedTime -= fFrameMaxTime;

	//Raptor: test fix for when WM_PAINT stops comming for a long time due to the
	//user moving the window, for instance. Tries to avoid the retrace trying to
	//catch up with the lost frame time
	if ((this->fElapsedTime / fFrameMaxTime) > MAX_FRAMESKIP_THRESHOLD)
		this->fElapsedTime = 0;

	if (this->iFpsTime > 1000)
	{
		Dbg("FPS: %d", this->iRetraceCount);

		arRetraceCount[iRetraceIndex++] = this->iRetraceCount;
		if (iRetraceIndex >= SYSTEM_RETRACE_HISTORY_MAX)
			iRetraceIndex = 0;

		this->iFpsTime -= 1000;
		this->iRetraceCount = 0;
	}
}

INLINE void System::SetFrameRate(eFrameRate rate)
{
	this->iFrameRate = rate;
}

INLINE ISystem::eFrameRate System::GetFrameRate()
{
	return this->iFrameRate;
}

INLINE void System::GoToMenu()
{
}

INLINE void System::OnHomeCalled()
{
}

INLINE void System::GoToDataManager()
{
}

INLINE void System::HangUp()
{
}

INLINE BOOL System::IsHomeEnabled() const
{
	return FALSE;
}

INLINE BOOL System::IsHomeRunning() const
{
	return FALSE;
}

INLINE BOOL System::InitializeHome()
{
	return TRUE;
}

INLINE void System::EnableHome()
{
}

INLINE void System::DisableHome()
{
}

INLINE void System::WriteOut(const char *msg)
{
	UNUSED(msg);
}

INLINE void System::WriteErr(const char *msg)
{
	UNUSED(msg);
}

INLINE void System::SetWidget(QWidget *widget)
{
	pWidget = widget;
}

}} // namespace

#endif // _QT_
