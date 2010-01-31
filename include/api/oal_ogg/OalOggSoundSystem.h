/*! \file OalOggSoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem OpenAL + Ogg Implementation
*/

#ifndef __OAL_OGG_SOUND_SYSTEM_H__
#define __OAL_OGG_SOUND_SYSTEM_H__

#include "Defines.h"

#ifdef _OAL_OGG_

#include "interface/ISoundSystem.h"
#include "interface/IResource.h"
#include "SoundSource.h"
#include "Music.h"
#include "File.h"
#include "Array.h"

#include <AL/al.h>

#define SOUND_MASTER_VOLUME		0.2f


namespace Seed { namespace OAL {


class SoundSystem : public ISoundSystem
{
	public:
		SoundSystem();
		virtual ~SoundSystem();

		// ISoundSystem
		//virtual void PlayMusic(IMusic *mus, f32 ms = 0);
		//virtual void StopMusic(f32 ms = 0, IMusic *mus = NULL);
		//virtual void StopSounds();
		virtual void Pause();
		virtual void Resume();

		//virtual void Add(ISoundSource *src);
		//virtual void Remove(ISoundSource *src);

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		static SoundSystem instance;

	private:
		SEED_DISABLE_COPY(SoundSystem);

		void UpdateMusic(f32 dt, IMusic *mus);
		void UpdateSounds(f32 dt);

	private:
		ALCdevice			*pDevice;
		ALCcontext			*pContext;
};


SoundSystem *const pSoundSystem = &SoundSystem::instance;


}} // namespace


#else // _OAL_OGG_

	#error "Include 'SoundSystem.h' instead 'api/oal_ogg/OalOggSoundSystem.h' directly."

#endif // _OAL_OGG_
#endif // __OAL_OGG_SOUND_SYSTEM_H__
