/*! \file IphThread.cpp
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/


#ifdef _IPHONE_

#include "IphThread.h"

#define TAG 	"[Thread] "


namespace Seed { namespace iPhone {


static int __seed_thread_loop_callback(void *param)
{
	Thread *pt = static_cast<Thread *>(param);
	while (pt->Run());

	return 0;
}


Thread::Thread()
	: pThread(NULL)
{
}


Thread::~Thread()
{
	if (pThread)
	{
		//SDL_WaitThread(pThread, NULL);
		//SDL_KillThread(pThread);
	}

	pThread = NULL;
}


INLINE void Thread::Create()
{
	//pThread = SDL_CreateThread(__seed_thread_loop_callback, this);
	//ASSERT_MSG(pThread != NULL, TAG "Failed to create thread.");
}


INLINE BOOL Thread::Run()
{
	return TRUE;
}


}} // namespace


#endif // _IPHONE_
