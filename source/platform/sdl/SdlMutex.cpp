/*! \file SdlMutex.cpp
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/


#ifdef _SDL_

#include <SDL/SDL_mutex.h>
#include "platform/sdl/SdlMutex.h"


namespace Seed { namespace SDL {


Mutex::Mutex()
	: pMutex(NULL)
{
	pMutex = SDL_CreateMutex();
	ASSERT_NULL(pMutex);
}

Mutex::~Mutex()
{
	SDL_DestroyMutex(pMutex);
}

INLINE void Mutex::Lock()
{
	IMutex::Lock();
	SDL_mutexP(pMutex);
}

INLINE void Mutex::Unlock()
{
	SDL_mutexV(pMutex);
	IMutex::Unlock();
}


}} // namespace


#endif // _SDL
