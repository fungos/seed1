/*! \file WiiSoundSource.h
	\author	Danny Angelo Carminati Grein
	\brief Sound source implementation for Wii platform using AX
*/

#ifndef __WII_SOUND_SOURCE_H__
#define __WII_SOUND_SOURCE_H__

#ifdef _WII_

#include "Sound.h"
#include "interface/ISoundSource.h"
#include "interface/ISound.h"
#include "platform/wii/WiiSoundHelper.h"

namespace Seed { namespace WII {

class SoundSystem;
struct sAdpcmInfo;

class SoundSource : public ISoundSource
{
	friend class SoundSystem;

	public:
		SoundSource();
		virtual ~SoundSource();

		// ISoundSource
		virtual void Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual void Unload();

		virtual void Play();
		virtual void Stop(f32 ms = 0);

	protected:
		Sound	*pSound;
		File	stFile;
		BOOL	bLoop;

	private:
		SEED_DISABLE_COPY(SoundSource);
};

}} // namespace

#else // _WII_
	#error "Include 'SoundSource.h' instead 'platform/wii/WiiSoundSource.h' directly."
#endif // _WII_
#endif // __WII_SOUND_SOURCE_H__
