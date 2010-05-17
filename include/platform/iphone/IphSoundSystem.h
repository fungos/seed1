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

/*! \file IphSoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem Iphone Implementation
*/

#ifndef __IPH_SOUND_SYSTEM_H__
#define __IPH_SOUND_SYSTEM_H__

#if defined(_IPHONE_)

#include "Config.h"
#include "interface/ISoundSystem.h"
#include "interface/IResource.h"
#include "File.h"
#include "Singleton.h"

#include <vector>
#include <OpenAL/al.h>

#define IPHONE_MAX_VOICES		32
#define IPHONE_MAX_FILES		64
#define IPHONE_MASTER_VOLUME	0.2f

namespace Seed { namespace iPhone {

IResource *AudioResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

class SoundSystem : public ISoundSystem, public IResource
{
	friend IResource *AudioResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

	SEED_SINGLETON_DECLARE(SoundSystem);
	public:
		// ISoundSystem
		virtual BOOL Prepare(void *workBuffer, u32 bufferLength);

		//virtual BOOL LoadPackage(const char *filename);
		virtual void Load(const char *filename, IMemoryPool *pool);

		virtual void PlaySound(u32 soundId);
		virtual void StopSound(u32 soundId, u16 fadeFrames);
		virtual void StopSounds();
		virtual void SetSoundVolume(f32 volume);

		virtual void PlayMusic(u32 musicId);
		virtual void StopMusic(u16 fadeFrames);
		virtual void PauseMusic();
		virtual void ResumeMusic();
		virtual void SetMusicVolume(f32 volume);

		virtual void Mute();
		virtual void Unmute();

		// IUpdatable
		virtual BOOL Update(f32 delta);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		typedef std::vector<const char *> MusicList;

		enum eSoundState
		{
			SOUND_STATE_STOPPED 	= 0,
			SOUND_STATE_START   	= 1,
			SOUND_STATE_STARTED 	= 2,
			SOUND_STATE_PLAYING 	= 3,
			SOUND_STATE_STOP		= 4,
			SOUND_STATE_FADING_IN	= 5,
			SOUND_STATE_FADING_OUT	= 6
		};

		struct sSoundFileInfo
		{
			ALsizei		iSize;
			ALsizei		iFreq;
			ALenum		eFormat;
			const char	*pFilename;
			void		*pData;
			ALuint		iBuffer;
		};

		struct sSoundVoiceInfo
		{
			u32			iId;
			ALuint		iSource;
			ALfloat		fVolume;
			eSoundState iState;
		};

	private:
		SEED_DISABLE_COPY(SoundSystem);

		void UpdateMusic();
		void UpdateSounds();
		void ReadData(sSoundFileInfo *obj, const char *file);

	private:
		File				stFile;
		void				*pAVPlayer;
		BOOL				bLoaded;

		void				*pData;
		const char			*pFilename;

		sSoundFileInfo		sndFile[IPHONE_MAX_FILES];
		sSoundVoiceInfo		sndVoice[IPHONE_MAX_VOICES];
		char				*musFile[IPHONE_MAX_FILES];
		//sSoundFileInfo		musFile[IPHONE_MAX_FILES];
		//sSoundVoiceInfo		musVoice;

		f32					fVolume;
		f32					fOriginalVolume;
		f32					fElapsedTime;
		u32					iFadeFrames;
		u32					iCurrentStream;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(SoundSystem);
}

}} // namespace

#else // _IPHONE_
	#error "Include 'SoundSystem.h' instead 'platform/iphone/IphSoundSystem.h' directly."
#endif // _IPHONE_
#endif // __IHP_SOUND_SYSTEM_H__
