/*! \file ITimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Timer class interface
*/

#include "interface/ITimer.h"
#include "Log.h"

namespace Seed {

ITimer::ITimer()
{
}

ITimer::~ITimer()
{
}

INLINE u64 ITimer::GetMilliseconds() const
{
	SEED_ABSTRACT_METHOD;
	return 0;
}

INLINE void ITimer::Sleep(u32 ms) const
{
	UNUSED(ms);
	SEED_ABSTRACT_METHOD;
}

INLINE BOOL ITimer::Reset()
{
	SEED_ABSTRACT_METHOD;
	return TRUE;
}

INLINE BOOL ITimer::IsRequired() const
{
	return TRUE;
}

INLINE const char *ITimer::GetObjectName() const
{
	return "Timer";
}

} // namespace
