/*! \file SdlMutex.h
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/


#ifndef __SDL_MUTEX_H__
#define __SDL_MUTEX_H__


#ifdef _SDL_

#include "interface/IMutex.h"


namespace Seed { namespace SDL {


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
		SDL_mutex		*pMutex;
};


}} // namespace


#else // _SDL_

	#error "Include 'Mutex.h' instead 'platform/sdl/SdlMutex.h' directly."

#endif // _SDL_
#endif // __SDL_MUTEX_H__
