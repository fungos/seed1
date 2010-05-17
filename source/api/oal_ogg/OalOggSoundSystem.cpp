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

/*! \file OalOggSoundSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem OpenAL + Ogg Implementation
*/

#include "SoundSystem.h"

#if defined(_OAL_OGG_)

#include "Log.h"
#include "MemoryManager.h"
#include "FileSystem.h"
#include "System.h"
#include "Formats.h"
#include "ResourceManager.h"
#include "SeedInit.h"
#include "Sound.h"
#include "SeedInit.h"
#include "interface/IMusic.h"
#include "interface/ISoundSource.h"
#include "Timer.h"

#include <stdio.h>
#include <algorithm>

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define TAG "[SoundSystem] "

#define AUDIO_FRAME_TIME	3
#define AUDIO_DATA_PATH		"/" FILESYSTEM_DEFAULT_PATH

// http://www.gamedev.net/reference/articles/article2031.asp
// http://icculus.org/SDL_sound/
// http://www.devmaster.net/articles.php?catID=6

namespace Seed { namespace OAL {

SEED_SINGLETON_DEFINE(SoundSystem);

SoundSystem::SoundSystem()
	: pDevice(NULL)
	, pContext(NULL)
{
	this->Reset();
}

SoundSystem::~SoundSystem()
{
}

INLINE BOOL SoundSystem::Initialize()
{
	Log(TAG "Initializing...");
	BOOL r = this->Reset();

	pDevice = alcOpenDevice(NULL);
	if (!pDevice)
	{
		Info(TAG "WARNING: Could not open OpenAL device - running wihtout sound!");
		//ASSERT_NULL(pDevice);
		r = FALSE;
	}
	else
	{
		pContext = alcCreateContext(pDevice, NULL);
		if (!pContext)
		{
			Info(TAG "WARNING: Could not create OpenAL context - running without sound!");
			//ASSERT_NULL(pContext);
			r = FALSE;
		}
		else
		{
			IModule::Initialize();
			alcMakeContextCurrent(pContext);

			ALCint v1 = 0;
			ALCint v2 = 0;

			alcGetIntegerv(pDevice, ALC_MAJOR_VERSION, 1, &v1);
			alcGetIntegerv(pDevice, ALC_MINOR_VERSION, 1, &v2);

			Info(TAG "OpenAL Version %d.%d", v1, v2);
			Info(TAG "Device: %s.", alcGetString(pDevice, ALC_DEFAULT_DEVICE_SPECIFIER));
			Info(TAG "Extensions: %s.", alcGetString(pDevice, ALC_EXTENSIONS));
		}
	}
	return r;
}

BOOL SoundSystem::Reset()
{
	if (bInitialized)
	{
		this->fMusicStartFadeTime = 0.0f;
		this->fMusicFadeTime = 0.0f;

		for (u32 i = arSource.Size(); i > 0; i--)
		{
			arSource[i-1]->Stop();
			arSource[i-1]->Unload();
		}

		this->StopMusic();
		pCurrentMusic = NULL;
		pNewMusic = NULL;

		arSource.Truncate();
		// abstract IModule::Reset();
	}
	return TRUE;
}

INLINE BOOL SoundSystem::Shutdown()
{
	BOOL r = TRUE;
	if (bInitialized)
	{
		Log(TAG "Terminating...");
		r = this->Reset();

		alcDestroyContext(pContext);
		alcCloseDevice(pDevice);

		IModule::Shutdown();
		Log(TAG "Terminated.");
	}
	return r;
}

INLINE BOOL SoundSystem::Update(f32 dt)
{
	if (bInitialized && !bPaused)
	{
		this->UpdateSounds(dt);

		if (pNewMusic)
			this->UpdateMusic(dt, pNewMusic);

		if (pCurrentMusic)
			this->UpdateMusic(dt, pCurrentMusic);
	}

	return TRUE;
}

void SoundSystem::UpdateSounds(f32 dt)
{
	UNUSED(dt);

	if (this->bChanged)
	{
		for (u32 i = 0; i < arSource.Size(); i++)
		{
			arSource[i]->UpdateVolume();
		}
	}

	for (u32 i = 0; i < arSource.Size(); i++)
	{
		SoundSource *src = static_cast<SoundSource *>(arSource[i]);

		eSoundSourceState state = src->GetState();
		if (state == SourceNone)
			continue;

		switch (state)
		{
			case Seed::SourcePlay:
			{
				src->eState = Seed::SourcePlayStarted;
			}
			break;

			case Seed::SourcePlayStarted:
			{
				src->eState = Seed::SourcePlaying;
			}
			break;

			case Seed::SourcePlaying:
			{
				ALint state = 0;
				alGetSourcei(src->iSource, AL_SOURCE_STATE, &state); // Quando volta do fadein essa porra ta ficando STOPPED!
				if (state == AL_STOPPED)
				{
					if (src->bLoop)
					{
						Log(TAG "Sound buffer underrun!");
						alSourcePlay(src->iSource);
					}
					else
					{
						Log(TAG "Source stopped...");
						src->eState = Seed::SourceStopped;
					}
				}
			}
			break;

			case Seed::SourceStop:
			{
				alSourceStop(src->iSource);
				src->eState = Seed::SourceStopped;
			}
			break;

			case Seed::SourceStopped:
			{
				src->eState = Seed::SourceNone;
			}
			break;

			case Seed::SourcePause:
			{
				src->eState = Seed::SourcePaused;
			}
			break;

			case Seed::SourcePaused:
			{
			}
			break;

			case Seed::SourceFadeIn:
			{
				src->eState = Seed::SourceFadingIn;
				alSourcePlay(src->iSource);
				alSourcef(src->iSource, AL_GAIN, 0.1f);
			}
			break;

			case Seed::SourceFadingIn:
			{
				f32 elapsed = static_cast<f32>(pTimer->GetMilliseconds() - src->fStartFadeTime);
				f32 volume = ((elapsed * src->fVolume) / src->fFadeTime);
				//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

				if (elapsed >= src->fFadeTime)
				{
					src->eState = Seed::SourcePlaying;
					alSourcef(src->iSource, AL_GAIN, src->fVolume * pSoundSystem->GetSfxVolume());
				}
				else
					alSourcef(src->iSource, AL_GAIN, volume * pSoundSystem->GetSfxVolume());
			}
			break;

			case Seed::SourceFadeOut:
			{
				src->eState = Seed::SourceFadingOut;
			}
			break;

			case Seed::SourceFadingOut:
			{
				f32 elapsed = src->fFadeTime - static_cast<f32>(pTimer->GetMilliseconds() - src->fStartFadeTime);
				f32 volume = ((elapsed * src->fVolume) / src->fFadeTime);
				//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

				if (elapsed <= 0.0f)
				{
					alSourcef(src->iSource, AL_GAIN, 0.0f);
					src->eState = Seed::SourceStopped;
					alSourceStop(src->iSource);
					alSourcef(src->iSource, AL_GAIN, src->fVolume * pSoundSystem->GetSfxVolume());
				}
				else
					alSourcef(src->iSource, AL_GAIN, volume * pSoundSystem->GetSfxVolume());
			}
			break;

			default:
			break;
		}
	}
}

INLINE void SoundSystem::UpdateMusic(f32 dt, IMusic *m)
{
	Music *mus = static_cast<Music *>(m);
	if (this->bChanged)
		mus->UpdateVolume();

	switch (mus->eState)
	{
		case Seed::MusicPlay:
		{
			mus->eState = Seed::MusicPlayStarted;
		}
		break;

		case Seed::MusicPlayStarted:
		{
			alSourcePlay(mus->iSource);
			mus->eState = Seed::MusicPlaying;
		}
		break;

		case Seed::MusicPlaying:
		{
			ALint state = 0;
			alGetSourcei(mus->iSource, AL_SOURCE_STATE, &state);
			if (state == AL_STOPPED)
			{
				if (mus->bLoop) // underrun happened
				{
					Log(TAG "Music buffer underrun!");
					ALint processed = 0;
					alGetSourcei(mus->iSource, AL_BUFFERS_PROCESSED, &processed);
					alSourcePlay(mus->iSource);
				}
				else // finished or underrung...
				{
					Log(TAG "Music stopped...");
					mus->eState = Seed::MusicStopped;
				}
			}
			else
			{
				mus->Update(dt);
			}
		}
		break;

		case Seed::MusicStop:
		{
			alSourceStop(mus->iSource);
			mus->eState = Seed::MusicStopped;
		}
		break;

		case Seed::MusicStopped:
		{
			mus->eState = Seed::MusicNone;
			if (mus == pCurrentMusic)
			{
				pCurrentMusic = pNewMusic;
				pNewMusic = NULL;
			}

			if (mus->bAutoUnload)
			{
				mus->Unload();
			}
			else
			{
				mus->Reset();
			}
		}
		break;

		case Seed::MusicPause:
		{
			mus->eState = Seed::MusicPaused;
		}
		break;

		case Seed::MusicPaused:
		{
			alSourcePause(mus->iSource);
		}
		break;

		case Seed::MusicFadeIn:
		{
			mus->eState = Seed::MusicFadingIn;
			alSourcef(mus->iSource, AL_GAIN, 0.1f);
			alSourcePlay(mus->iSource);
		}
		break;

		case Seed::MusicFadingIn:
		{
			f32 elapsed = static_cast<f32>(pTimer->GetMilliseconds() - this->fMusicStartFadeTime);
			f32 volume = ((elapsed * mus->fVolume) / this->fMusicFadeTime);
			//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

			if (elapsed >= this->fMusicFadeTime)
			{
				mus->eState = Seed::MusicPlaying;
				alSourcef(mus->iSource, AL_GAIN, mus->fVolume * pSoundSystem->GetMusicVolume());
			}
			else
			{
				alSourcef(mus->iSource, AL_GAIN, volume * pSoundSystem->GetMusicVolume());
			}
			mus->Update(dt);
		}
		break;

		case Seed::MusicFadeOut:
		{
			mus->eState = Seed::MusicFadingOut;
		}
		break;

		/* FIXME: 2009-15-06 | BUG | SDL | Fadeout / Fadein nao estao funcionando (alSourcef AL_GAIN) */
		case Seed::MusicFadingOut:
		{
			f32 elapsed = this->fMusicFadeTime - static_cast<f32>(pTimer->GetMilliseconds() - this->fMusicStartFadeTime);
			f32 volume = ((elapsed * mus->fVolume) / this->fMusicFadeTime);
			//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

			if (elapsed <= 0.0f)
			{
				alSourcef(mus->iSource, AL_GAIN, 0.0f);
				mus->eState = Seed::MusicStopped;
			}
			else
			{
				alSourcef(mus->iSource, AL_GAIN, volume * pSoundSystem->GetMusicVolume());
			}
		}
		break;

		case Seed::MusicNone:
		default:
		break;
	}
}

INLINE void SoundSystem::Pause()
{
	ISoundSystem::Pause();

	for (u32 i = 0; i < arSource.Size(); i++)
	{
		SoundSource *src = static_cast<SoundSource *>(arSource[i]);
		src->Pause();
		alSourcePause(src->iSource);
	}

	if (pCurrentMusic)
		static_cast<Music *>(pCurrentMusic)->eState = Seed::MusicPaused;

	if (pNewMusic)
		static_cast<Music *>(pNewMusic)->eState = Seed::MusicPaused;
}

INLINE void SoundSystem::Resume()
{
	for (u32 i = 0; i < arSource.Size(); i++)
	{
		arSource[i]->Resume();
	}

	if (pCurrentMusic)
		static_cast<Music *>(pCurrentMusic)->eState = Seed::MusicPlay;

	if (pNewMusic)
		static_cast<Music *>(pNewMusic)->eState = Seed::MusicPlay;

	ISoundSystem::Resume();
}

}} // namespace

#endif // _OAL_OGG_
