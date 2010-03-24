/*! \file IphMutex.h
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/

#ifndef __IPH_MUTEX_H__
#define __IPH_MUTEX_H__

#ifdef _IPHONE_

#include "interface/IMutex.h"

namespace Seed { namespace iPhone {

class Mutex : public IMutex
{
	public:
		Mutex();
		virtual ~Mutex();

		virtual void Lock();
		virtual void Unlock();

	private:
		SEED_DISABLE_COPY(Mutex);

	private:
		void		*pMutex;
};

}} // namespace

#else // _IPHONE_
	#error "Include 'Mutex.h' instead 'platform/iphone/IphMutex.h' directly."
#endif // _IPHONE_
#endif // __IPH_MUTEX_H__
