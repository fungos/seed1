/*! \file IThread.cpp
	\author	Danny Angelo Carminati Grein
	\brief Thread interface
*/

#include "interface/IThread.h"


namespace Seed {


IThread::IThread()
{
}

IThread::~IThread()
{
}

INLINE void IThread::Create(s32 priority)
{
	UNUSED(priority)
}

INLINE void IThread::Destroy()
{
}

INLINE BOOL IThread::Run()
{
	return TRUE;
}


} // namespace
