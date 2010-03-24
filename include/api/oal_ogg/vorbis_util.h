#ifndef __VORBIS_UTIL_H__
#define __VORBIS_UTIL_H__

#if defined(_SDL_) || defined(_QT_)

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#include "Defines.h"

// 40kb para que em maquinas lentas nao fique engasgando no som
#define VORBIS_BUFFER_SIZE SEED_MUSIC_STREAMING_BUFFER_SIZE


struct sOggFile
{
	u8		*dataPtr;
	u32		dataSize;
	u64		dataRead;

	sOggFile() : dataPtr(NULL), dataSize(0), dataRead(0) {}
};


size_t vorbis_read(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource);
int vorbis_seek(void *datasource, ogg_int64_t offset, int whence);
int vorbis_close(void *datasource);
long vorbis_tell(void *datasource);
bool ogg_update_stream(OggVorbis_File *oggStream, ogg_int64_t rate, ALenum format, ALuint buffer, bool loop);

#endif // _SDL_ || _QT_

#endif // __VORBIS_UTIL_H__
