/*! \file OalOggSound.h
	\author	Danny Angelo Carminati Grein
	\brief Sound Implementation for OpenAL + Ogg
*/


#ifndef __OAL_OGG_SOUND_H__
#define __OAL_OGG_SOUND_H__

#include "Defines.h"

#ifdef _OAL_OGG_

#include "File.h"
#include "SeedInit.h"
#include "interface/IResource.h"
#include "interface/ISound.h"
#include "api/oal_ogg/vorbis_util.h"

#include <AL/al.h>


namespace Seed { namespace OAL {


IResource *SoundResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);


class Sound : public ISound
{
	friend IResource *SoundResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class SoundSystem;
	friend class SoundSource;

	public:
		Sound();
		virtual ~Sound();

		void Reset();

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		virtual const void *GetData() const;

	private:
		SEED_DISABLE_COPY(Sound);

	private:
		ALuint			iBuffer;
		File			stFile;
};


}} // namespace


#else // _OAL_OGG_

	#error "Include 'Sound.h' instead 'api/oal_ogg/OalOggSound.h' directly."

#endif // _OAL_OGG_
#endif // __OAL_OGG_SOUND_H__
