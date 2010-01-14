/*! \file ITimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Timer class interface
*/

#include "interface/ITimer.h"


namespace Seed {


ITimer::ITimer()
{
}

ITimer::~ITimer()
{
}

INLINE u64 ITimer::GetMilliseconds() const
{
	return 0;
}

INLINE void ITimer::Sleep(u32 ms) const
{
	UNUSED(ms);
}

INLINE BOOL ITimer::Reset()
{
	return TRUE;
}

} // namespace
