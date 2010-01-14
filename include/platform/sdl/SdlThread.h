/*! \file SdlThread.h
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/

#ifndef __SDL_THREAD_H__
#define __SDL_THREAD_H__


#ifdef _SDL_


#include "Defines.h"
#include "interface/IThread.h"


namespace Seed { namespace SDL {


class Thread : public IThread
{
	public:
		Thread();
		virtual ~Thread();

		virtual void Create(s32 priority = 31);
		virtual void Destroy();

		virtual BOOL Run();

	private:
		SEED_DISABLE_COPY(Thread);

	private:
		BOOL		bRunning;
		SDL_Thread 	*pThread;
};


}} // namespace


#else // _SDL_

	#error "Include 'Thread.h' instead 'platform/sdl/SdlThread.h' directly."

#endif // _SDL_
#endif // __SDL_THREAD_H__
