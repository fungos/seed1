/*! \file WiiSound.h
	\author	Danny Angelo Carminati Grein
	\brief Sound Implementation for Wii
*/


#ifndef __WII_SOUND_H__
#define __WII_SOUND_H__


#ifdef _WII_


#include "Defines.h"
#include "File.h"
#include "SeedInit.h"
#include "interface/IResource.h"
#include "interface/ISound.h"


namespace Seed { namespace WII {


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
		virtual u32 GetUsedMemory() const;
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

	protected:
		virtual const void *GetData() const;

	protected:
		File			stFile;
		BOOL			bLoop;

	private:
		SEED_DISABLE_COPY(Sound);
};


}} // namespace


#else // _WII_

	#error "Include 'Sound.h' instead 'platform/wii/WiiSound.h' directly."

#endif // _WII_
#endif // __WII_SOUND_H__
