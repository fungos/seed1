/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

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
