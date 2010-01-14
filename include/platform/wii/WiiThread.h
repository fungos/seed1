/*! \file WiiThread.h
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/

#ifndef __WII_THREAD_H__
#define __WII_THREAD_H__


#include "Defines.h"


#if defined(_WII_)


#include "interface/IThread.h"


namespace Seed { namespace WII {


class Thread : public IThread
{
	public:
		Thread();
		virtual ~Thread();

		virtual void Create(s32 priority = 31);
		virtual BOOL Run();

	private:
		SEED_DISABLE_COPY(Thread);

	private:
		WiiThread 	stThread;
		u8 			arThreadStack[PLATFORM_THREAD_STACK_SIZE];
};


}} // namespace


#else // _WII_

	#error "Include 'Thread.h' instead 'platform/wii/WiiThread.h' directly."

#endif // _WII_
#endif // __WII_THREAD_H__
