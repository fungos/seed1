/*! \file IThread.h
	\author	Danny Angelo Carminati Grein
	\brief Thread interface
*/

#ifndef __ITHREAD_H__
#define __ITHREAD_H__


#include "Defines.h"


namespace Seed {


/// Thread interface
/**
Thread interface
*/
class IThread
{
	public:
		IThread();
		virtual ~IThread();

		virtual void Create(s32 priority = 31);
		virtual void Destroy();
		virtual BOOL Run();

	private:
		SEED_DISABLE_COPY(IThread);
};


} // namespace


#endif // __ITHREAD_H__
