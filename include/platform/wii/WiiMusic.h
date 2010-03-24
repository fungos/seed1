/*! \file WiiMusic.h
	\author	Danny Angelo Carminati Grein
	\brief Music Implementation for Wii
*/

#ifndef __WII_MUSIC_H__
#define __WII_MUSIC_H__

#ifdef _WII_

#include "Defines.h"
#include "File.h"
#include "interface/IMusic.h"
#include "SeedInit.h"

#define MAX_FILENAME	64

namespace Seed { namespace WII {

IResource *MusicResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);

class Music : public IMusic
{
	friend IResource *MusicResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class SoundSystem;

	public:
		Music();
		virtual ~Music();

		// IMusic
		virtual BOOL Update(f32 delta);

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

	protected:
		virtual void Play();
		virtual void Stop();

	private:
		SEED_DISABLE_COPY(Music);

	private:
		File			stMusicFile;
		File			stFileLeftChannel;
		File			stFileRightChannel;

		WiiVoiceData	*pVoiceLeftChannel;
		WiiVoiceData	*pVoiceRightChannel;

		//char			pFileLeftChannel[MAX_FILENAME];
		//char			pFileRightChannel[MAX_FILENAME];
};

}} // namespace

#else // _WII_
	#error "Include 'Music.h' instead 'platform/wii/WiiMusic.h' directly."
#endif // _WII_
#endif // __WII_MUSIC_H__
