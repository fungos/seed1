/*! \file QtMutex.h
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/


#ifndef __QT_MUTEX_H__
#define __QT_MUTEX_H__


#ifdef _QT_

#include "interface/IMutex.h"
#include <QMutex>


namespace Seed { namespace QT {


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
		QMutex	 mutex;
};


}} // namespace


#else // _QT_

	#error "Include 'Mutex.h' instead 'platform/qt/QtMutex.h' directly."

#endif // _QT_
#endif // __QT_MUTEX_H__
