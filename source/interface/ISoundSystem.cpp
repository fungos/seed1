/*! \file ISoundSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Sound System class interface
*/


#include "interface/ISoundSystem.h"
#include "interface/IMusic.h"
#include "Timer.h"

namespace Seed {


ISoundSystem::ISoundSystem()
	: fMusicVolume(1.0f)
	, fSfxVolume(1.0f)
	, fMusicVolumeOrig(1.0f)
	, fSfxVolumeOrig(1.0f)
	, bMuted(FALSE)
	, bChanged(FALSE)
	, bPaused(FALSE)
	, arSource()
	, pCurrentMusic(NULL)
	, pNewMusic(NULL)
	, fMusicStartFadeTime(0.0f)
	, fMusicFadeTime(0.0f)
{
}

ISoundSystem::~ISoundSystem()
{
}

INLINE void ISoundSystem::Add(ISoundSource *src)
{
	arSource.Add(static_cast<ISoundSource *>(src));
}

INLINE void ISoundSystem::Remove(ISoundSource *src)
{
	arSource.Remove(static_cast<ISoundSource *>(src));
}

INLINE void ISoundSystem::SetMusicVolume(f32 volume)
{
	ASSERT_MSG((volume >= 0 || volume <= 1.0f), "Music volume must be between 0 and 1");
	this->fMusicVolume = volume;
	this->bChanged = TRUE;
}

INLINE f32 ISoundSystem::GetMusicVolume() const
{
	return this->fMusicVolume;
}

INLINE void ISoundSystem::SetSfxVolume(f32 volume)
{
	ASSERT_MSG((volume >= 0 || volume <= 1.0f), "Sfx volume must be between 0 and 1");
	this->fSfxVolume = volume;
	this->bChanged = TRUE;
}

INLINE f32 ISoundSystem::GetSfxVolume() const
{
	return this->fSfxVolume;
}

INLINE void ISoundSystem::Mute()
{
	if (!this->bMuted)
	{
		this->fMusicVolumeOrig = this->fMusicVolume;
		this->fSfxVolumeOrig = this->fSfxVolume;
		this->bMuted = TRUE;
		this->SetMusicVolume(0.0f);
		this->SetSfxVolume(0.0f);
	}
}

INLINE void ISoundSystem::Unmute()
{
	if (this->bMuted)
	{
		this->bMuted = FALSE;
		this->SetMusicVolume(this->fMusicVolumeOrig);
		this->SetSfxVolume(this->fSfxVolumeOrig);
	}
}

INLINE void ISoundSystem::PlayMusic(IMusic *mus, f32 ms)
{
	fMusicFadeTime = ms;
	fMusicStartFadeTime = static_cast<f32>(pTimer->GetMilliseconds());

	if (pCurrentMusic && mus != pCurrentMusic)
	{
		if (pNewMusic)
			pNewMusic->Stop();

		pNewMusic = mus;

		if (ms)
		{
			pCurrentMusic->eState = Seed::MusicFadeOut;
			pNewMusic->eState = Seed::MusicFadeIn;
		}
		else
		{
			pCurrentMusic->eState = Seed::MusicStop;
			pNewMusic->eState = Seed::MusicPlay;
		}
	}
	else
	{
		pCurrentMusic = mus;

		if (ms)
		{
			pCurrentMusic->eState = Seed::MusicFadeIn;
		}
		else
		{
			pCurrentMusic->eState = Seed::MusicPlay;
		}
	}
}

INLINE void ISoundSystem::StopMusic(f32 ms, IMusic *mus)
{
	if (mus)
	{
		mus->eState = Seed::MusicStop;

		if (pCurrentMusic == mus)
			pCurrentMusic = NULL;

		if (pNewMusic == mus)
			pNewMusic = NULL;
	}
	else if (pCurrentMusic)
	{
		this->fMusicFadeTime = ms;
		if (ms)
		{
			fMusicStartFadeTime = static_cast<f32>(pTimer->GetMilliseconds());
			pCurrentMusic->eState = Seed::MusicFadeOut;
		}
		else
		{
			pCurrentMusic->eState = Seed::MusicStop;
		}
	}
}

INLINE void ISoundSystem::StopSounds()
{
	for (u32 i = 0; i < arSource.Size(); i++)
	{
		arSource[i]->Stop();
	}
}

INLINE void ISoundSystem::Pause()
{
	this->bPaused = TRUE;
}

INLINE void ISoundSystem::Resume()
{
	this->bPaused = FALSE;
}

INLINE const char *ISoundSystem::GetObjectName() const
{
	return "SoundSystem";
}

} // namespace
