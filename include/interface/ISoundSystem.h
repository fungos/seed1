/*! \file ISoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the SoundSystem class interface
*/

#ifndef __ISOUND_SYSTEM_H__
#define __ISOUND_SYSTEM_H__

#include "IModule.h"
#include "IUpdatable.h"
#include "IMemoryPool.h"
#include "ISoundSource.h"
#include "../Array.h"


#define DEFAULT_FADE_OUT		15
#define SOUND_MAX_SOURCES		32


namespace Seed {


class ISound;
class IMusic;

/// Sound Interface
/**
Interface for sound implementations.
*/
class ISoundSystem : public IModule, public IUpdatable
{
	public:
		ISoundSystem();
		virtual ~ISoundSystem();

		virtual void SetMusicVolume(f32 volume);
		virtual f32 GetMusicVolume() const;

		virtual void SetSfxVolume(f32 volume);
		virtual f32 GetSfxVolume() const;

		virtual void Mute();
		virtual void Unmute();

		virtual void PlayMusic(IMusic *mus, f32 ms = 0);
		virtual void StopMusic(f32 ms = 0, IMusic *mus = NULL);
		virtual void StopSounds();
		virtual void Pause();
		virtual void Resume();

		virtual void Add(ISoundSource *src);
		virtual void Remove(ISoundSource *src);

	protected:
		f32 	fMusicVolume;
		f32		fSfxVolume;
		f32 	fMusicVolumeOrig;
		f32		fSfxVolumeOrig;
		BOOL	bMuted;
		BOOL	bChanged;
		BOOL	bPaused;

		Array<ISoundSource *, SOUND_MAX_SOURCES> arSource;
		IMusic				*pCurrentMusic;
		IMusic				*pNewMusic;

		f32					fMusicStartFadeTime;
		f32					fMusicFadeTime;

	private:
		SEED_DISABLE_COPY(ISoundSystem);
};


} // namespace


#endif // __ISOUND_SYSTEM_H__
