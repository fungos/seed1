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

	public:
		SoundSystem();
		virtual ~SoundSystem();

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
		virtual BOOL Update();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static SoundSystem instance;

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

SoundSystem *const pSoundSystem = &SoundSystem::instance;


}} // namespace


#else // _WII_

	#error "Include 'SoundSystem.h' instead 'platform/wii/WiiSoundSystem.h' directly."

#endif // _WII_
#endif // __WII_SOUND_SYSTEM_H__
