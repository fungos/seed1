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
#define SOUND_MAX_SOURCES		128

namespace Seed {

class ISound;
class IMusic;

/// Sound Interface
/**
Interface for sound implementations.
*/
class SEED_CORE_API ISoundSystem : public IModule, public IUpdatable
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

		// IObject
		virtual const char *GetObjectName() const;

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
