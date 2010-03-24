/*! \file IphSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief System Iphone Implementation
*/

#ifdef _IPHONE_

#include "Defines.h"
#include "System.h"
#include "Log.h"
#include "MemoryManager.h"
#include "IphoneView.h"

#define TAG "[System] "

namespace Seed { namespace iPhone {

System System::instance;

System::System()
	: iRetraceCount(0)
	, iFrameRate(RATE_60FPS)
{
}

System::~System()
{
}

BOOL System::Reset()
{
	return TRUE;
}

BOOL System::Initialize()
{
	Log(TAG "Initializing...");
	Log(TAG "Initialization completed.");

	return TRUE;
}

BOOL System::Shutdown()
{
	Log(TAG "Terminated.");
	Log(TAG "Terminated.");

	return TRUE;
}

BOOL System::Update(f32 delta)
{
	UNUSED(delta);

	this->WaitForRetrace(this->iFrameRate);

	return TRUE;
}

void System::Sleep()
{
	Log(TAG "WARNING: Platform doesnt support sleep mode.");
}

INLINE BOOL System::IsSleeping() const
{
	return FALSE;
}

INLINE BOOL System::IsShuttingDown() const
{
	return FALSE;
}

INLINE BOOL System::IsResetting() const
{
	return FALSE;
}

INLINE void System::WaitForRetrace(eFrameRate rate)
{
	UNUSED(rate);
	// This platform is synced by NSTimer at AppView
	this->iRetraceCount = 0;
}

INLINE void System::SetFrameRate(eFrameRate rate)
{
	double v = static_cast<double> (rate);
	v = 1.0 / v;

	iphSetUpdateRate(v);

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

INLINE void System::DisableHome()
{
}

INLINE void System::EnableHome()
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
	return FALSE;
}

}} // namespace

#endif // _IPHONE_
