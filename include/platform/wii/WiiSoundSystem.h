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

/*! \file WiiSoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem functionality
*/

#ifndef __WII_SOUND_SYSTEM_H__
#define __WII_SOUND_SYSTEM_H__

#include "Defines.h"

#if defined(_WII_)

#include "Music.h"
#include "interface/ISoundSystem.h"
#include "File.h"
#include "interface/IResource.h"
#include "Array.h"
#include "SoundSource.h"
#include "platform/wii/WiiSoundHelper.h"
#include "Singleton.h"

#define SOUND_MASTER_VOLUME_FACTOR		1.4f

namespace Seed { namespace WII {

extern "C"
{
	static void sounds_frame_callback();
}

class SoundSystem : public ISoundSystem
{
	friend class SoundSource;
	friend void sounds_frame_callback();

	SEED_SINGLETON_DECLARE(SoundSystem);
	public:
		// ISoundSystem
		//virtual void PlayMusic(IMusic *mus, f32 ms = 0);
		//virtual void StopMusic(f32 ms = 0, IMusic *mus = NULL);
		virtual void StopSounds();
		virtual void Pause();
		virtual void Resume();

		virtual f32  GetMusicVolume() const;

		//virtual void Add(ISoundSource *src);
		//virtual void Remove(ISoundSource *src);

		// IUpdatable
		virtual BOOL Update(f32 delta);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	protected:
		struct VoiceInfo
		{
			WiiVoiceData       *pVoice;
			SoundSource *pOwner;
			sAdpcmInfo	sInfo;
			eSoundSourceState eState;
			f32			fVolume;
			f32			fStartFadeTime;
			f32			fFadeTime;
		};

		void PlayVoice(SoundSource *src);
		void StopVoice(SoundSource *src, f32 ms);
		void SetVoiceVolume(VoiceInfo *info, f32 vol);
		void SetMusicVoiceVolume(Music *mus, f32 vol);

		void UpdateCallback();

	private:
		SEED_DISABLE_COPY(SoundSystem);

		void UpdateSounds();
		void UpdateMusic(Music *mus);

	private:
		VoiceInfo	arVoices[WII_MAX_VOICES];
		void 		*pMixBuffer;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(SoundSystem);
}

}} // namespace

#else // _WII_
	#error "Include 'SoundSystem.h' instead 'platform/wii/WiiSoundSystem.h' directly."
#endif // _WII_
#endif // __WII_SOUND_SYSTEM_H__
