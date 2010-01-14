/*! \file IMusic.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Music class interface
*/

#ifndef __IMUSIC_H__
#define __IMUSIC_H__


#include "SeedInit.h"
#include "IResource.h"
#include "MemoryManager.h"
#include "ResourceManager.h"

namespace Seed {


enum eMusicState
{
	MusicNone,
	MusicPlay,
	MusicPlayStarted,
	MusicPlaying,
	MusicStop,
	MusicStopped,
	MusicPause,
	MusicPaused,
	MusicFadeIn,
	MusicFadingIn,
	MusicFadeOut,
	MusicFadingOut,
	MusicFading
};


class IMusic : public IResource
{
	friend class ISoundSystem;
	public:
		IMusic();
		virtual ~IMusic();

		virtual BOOL Update(f32 dt);
		virtual const void *GetData() const;

		virtual void SetVolume(f32 volume);
		virtual f32 GetVolume() const;
		virtual void SetAutoUnload(BOOL b);
		virtual BOOL IsPlaying() const;

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool);
		virtual BOOL Unload();

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		virtual void Play();
		virtual void Stop();
		virtual void Pause();

		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [] (void *);

	protected:
		f32					fVolume;
		eMusicState			eState;
		BOOL				bAutoUnload;

	private:
		SEED_DISABLE_COPY(IMusic);
};


} // namespace

#endif // __IMUSIC_H__
