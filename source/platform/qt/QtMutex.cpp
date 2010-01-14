/*! \file QtMutex.cpp
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/


#ifdef _QT_

#include "platform/qt/QtMutex.h"


namespace Seed { namespace QT {


Mutex::Mutex()
{
}

Mutex::~Mutex()
{
}

INLINE void Mutex::Lock()
{
	IMutex::Lock();
	mutex.lock();
}

INLINE void Mutex::Unlock()
{
	mutex.unlock();
	IMutex::Unlock();
}

INLINE BOOL Mutex::TryLock()
{
	return mutex.tryLock();
}


}} // namespace


#endif // _QT_
