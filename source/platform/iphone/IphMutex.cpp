/*! \file IphMutex.cpp
	\author	Danny Angelo Carminati Grein
	\brief Mutex
*/


#ifdef _IPHONE_


#include "IphMutex.h"


namespace Seed { namespace iPhone {


Mutex::Mutex()
	: pMutex(NULL)
{
	//pMutex = SDL_CreateMutex();
	//ASSERT_NULL(pMutex);
}


Mutex::~Mutex()
{
	//SDL_DestroyMutex(pMutex);
}


INLINE void Mutex::Lock()
{
	IMutex::Lock();
	//SDL_mutexP(pMutex);
}


INLINE void Mutex::Unlock()
{
	//SDL_mutexV(pMutex);
	IMutex::Unlock();
}


}} // namespace


#endif // _IPHONE_
