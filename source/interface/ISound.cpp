/*! \file ISound.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Sound class interface
*/

#include "interface/ISound.h"


namespace Seed {


ISound::ISound()
{
}

ISound::~ISound()
{
}

INLINE void *ISound::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

INLINE void ISound::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}

INLINE int ISound::GetObjectType() const
{
	return Seed::ObjectSound;
}

INLINE const char *ISound::GetObjectName() const
{
	return "ISound";
}


} // namespace

