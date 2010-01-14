/*! \file OalOggSoundSource.h
	\author	Danny Angelo Carminati Grein
	\brief Sound source implementation using OpenAL API
*/

#ifndef __OAL_OGG_SOUND_SOURCE_H__
#define __OAL_OGG_SOUND_SOURCE_H__

#include "Defines.h"

#ifdef _OAL_OGG_

#include "Sound.h"
#include "interface/ISoundSource.h"
#include "interface/ISound.h"
#include "File.h"

#include <AL/al.h>
#include <AL/alc.h>


namespace Seed { namespace OAL {


class SoundSource : public ISoundSource
{
	friend class SoundSystem;

	public:
		SoundSource();
		virtual ~SoundSource();

		// ISoundSource
		virtual void Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual void Unload();

		virtual void SetLoop(BOOL b);
		virtual void Play();
		virtual void Resume();

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

	private:
		SEED_DISABLE_COPY(SoundSource);

	private:
		ALuint			iSource;
		Sound			*pSound;
		File			stFile;
};


}} // namespace

#else // _OAL_OGG_
	
	#error "Include 'SoundSource.h' instead 'api/oal_ogg/OalOggSoundSource.h' directly."

#endif // _OAL_OGG_
#endif // __OAL_OGG_SOUND_SOURCE_H__
