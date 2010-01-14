/*! \file IphSoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem Iphone Implementation
*/

#ifndef __IPH_SOUND_SYSTEM_H__
#define __IPH_SOUND_SYSTEM_H__

#ifdef _IPHONE_

#include "Config.h"
#include "interface/ISoundSystem.h"
#include "interface/IResource.h"
#include "File.h"

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

	public:
		SoundSystem();
		virtual ~SoundSystem();

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
		virtual BOOL Update(f32 dt);

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


	public:
		static SoundSystem instance;


	private:
		SoundSystem(const SoundSystem &);
		SoundSystem &operator=(const SoundSystem &);

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


SoundSystem *const pSoundSystem = &SoundSystem::instance;


}} // namespace


#else // _IPHONE_

	#error "Include 'SoundSystem.h' instead 'platform/iphone/IphSoundSystem.h' directly."

#endif // _IPHONE_
#endif // __IHP_SOUND_SYSTEM_H__
