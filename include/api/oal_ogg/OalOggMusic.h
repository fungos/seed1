/*! \file OalOggMusic.h
	\author	Danny Angelo Carminati Grein
	\brief Music Implementation for OpenAL + Ogg
*/


#ifndef __OAL_OGG_MUSIC_H__
#define __OAL_OGG_MUSIC_H__


#include "Defines.h"

#ifdef _OAL_OGG_

#include "File.h"
#include "interface/IMusic.h"
#include "SeedInit.h"
#include "Sound.h"
#include "api/oal_ogg/vorbis_util.h"


#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define OPENAL_MUSIC_BUFFERS	2

namespace Seed { namespace OAL {


IResource *MusicResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);


class Music : public IMusic
{
	friend IResource *MusicResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class SoundSystem;

	public:
		Music();
		virtual ~Music();

		// IMusic
		virtual void Reset();
		virtual BOOL Update(f32 dt);
		virtual const void *GetData() const;

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

		// IResouce
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

	protected:
		//virtual void Play();
		//virtual void Stop();
	
		virtual BOOL DoStream(ALuint buffer);

	private:
		SEED_DISABLE_COPY(Music);

	private:
		ALuint			iBuffers[OPENAL_MUSIC_BUFFERS];
		ALuint			iSource;

		vorbis_info		*vorbisInfo;
		vorbis_comment	*vorbisComment;
		OggVorbis_File	oggStream;
		ALenum			eFormat;
		BOOL			bLoop;
		File			stFile;
};


}} // namespace


#else // _OAL_OGG_

	#error "Include 'Music.h' instead 'api/oal_ogg/OalOggMusic.h' directly."

#endif // _OAL_OGG_
#endif // __OAL_OGG_MUSIC_H__
