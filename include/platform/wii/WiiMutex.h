/*! \file WiiMutex.h
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/

#ifndef __WII_MUTEX_H__
#define __WII_MUTEX_H__

#include "Defines.h"

#if defined(_WII_)

#include "interface/IMutex.h"

namespace Seed { namespace WII {

class Mutex : public IMutex
{
	public:
		Mutex();
		virtual ~Mutex();

		virtual void Lock();
		virtual void Unlock();
		virtual BOOL TryLock();

	private:
		SEED_DISABLE_COPY(Mutex);

	private:
		WiiMutex		stMutex;
};

}} // namespace

#else // _WII_
	#error "Include 'Mutex.h' instead 'platform/wii/WiiMutex.h' directly."
#endif // _WII_
#endif // __WII_MUTEX_H__
