/*! \file SdlThread.cpp
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/


#ifdef _SDL_

#include "platform/sdl/SdlThread.h"

#define TAG 	"[Thread] "


namespace Seed { namespace SDL {


static int __seed_thread_loop_callback(void *param)
{
	Thread *pt = static_cast<Thread *>(param);
	while (pt->Run());

	pt->Destroy(); // Sera q fode?
	return 0;
}

Thread::Thread()
	: bRunning(TRUE)
	, pThread(NULL)
{
}

Thread::~Thread()
{
	this->Destroy();
}

INLINE void Thread::Create(s32 priority)
{
	UNUSED(priority);

	bRunning = TRUE;
	pThread = SDL_CreateThread(__seed_thread_loop_callback, this);
	ASSERT_MSG(pThread != NULL, TAG "Failed to create thread.");
}

INLINE void Thread::Destroy()
{
	bRunning = FALSE;
	if (pThread)
	{
#if defined(__APPLE_CC__)
		SDL_WaitThread(pThread, NULL);
#else
		SDL_KillThread(pThread);
#endif
	}

	pThread = NULL;
}

INLINE BOOL Thread::Run()
{
	return bRunning;
}


}} // namespace


#endif // _SDL_
