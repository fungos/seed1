/*! \file IMutex.h
	\author	Danny Angelo Carminati Grein
	\brief Interface to a mutex object
*/

#ifndef __IMUTEX_H__
#define __IMUTEX_H__


#include "Defines.h"


namespace Seed {


/// Object Interface
/**
Interface for a simple mutex object
*/
class IMutex
{
	public:
		IMutex();
		virtual ~IMutex();

		/// Lock mutex
		virtual void Lock();

		/// Unlock mutex
		virtual void Unlock();

		/// TryLock mutex
		virtual BOOL TryLock();

	protected:
		BOOL bLocked;

	private:
		SEED_DISABLE_COPY(IMutex);
};


} // namespace


#endif // __IMUTEX_H__
