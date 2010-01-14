/*! \file IMusic.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Music class interface
*/

#include "interface/IMusic.h"


namespace Seed {


IMusic::IMusic()
	: fVolume(1.0f)
	, eState(Seed::MusicNone)
	, bAutoUnload(FALSE)
{
}

IMusic::~IMusic()
{
}

INLINE BOOL IMusic::Unload()
{
	return FALSE;
}

INLINE BOOL IMusic::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(filename);
	UNUSED(res);
	UNUSED(pool);
	return FALSE;
}

INLINE BOOL IMusic::Update(f32 dt)
{
	UNUSED(dt);

	return FALSE;
}

INLINE const void *IMusic::GetData() const
{
	return NULL;
}

INLINE void IMusic::SetVolume(f32 volume)
{
	this->fVolume = volume;
}

INLINE f32 IMusic::GetVolume() const
{
	return this->fVolume;
}

INLINE void IMusic::SetAutoUnload(BOOL b)
{
	this->bAutoUnload = b;
}

INLINE void *IMusic::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

INLINE void IMusic::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}

INLINE int IMusic::GetObjectType() const
{
	return Seed::ObjectMusic;
}

INLINE const char *IMusic::GetObjectName() const
{
	return "IMusic";
}

INLINE void IMusic::Play()
{
}

INLINE void IMusic::Stop()
{
}

INLINE void IMusic::Pause()
{
}

INLINE BOOL IMusic::IsPlaying() const
{
	return ((eState != MusicStopped) &&
			(eState != MusicStop) &&
			(eState != MusicPause) &&
			(eState != MusicPaused) &&
			(eState != MusicNone));
}


} // namespace
