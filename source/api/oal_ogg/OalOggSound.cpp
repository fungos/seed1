/*! \file OalOggSound.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sound Implementation for OpenAL + Ogg
*/

#include "Sound.h"

#ifdef _OAL_OGG_

#include "Log.h"


#define TAG "[Sound] "
#define BUFFER_SIZE		8512

namespace Seed { namespace OAL {


IResource *SoundResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);

	Sound *sound = new Sound();
	sound->Load(filename, res, pool);
	return sound;
}

Sound::Sound()
	: iBuffer(0)
	, stFile()
{
}

Sound::~Sound()
{
	this->Unload();
}

INLINE void Sound::Reset()
{
	this->Unload();
}

BOOL Sound::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	if (this->Unload())
	{
		this->pFilename = filename;
		this->pRes = res;
		this->pPool = pool;

		OggVorbis_File	oggStream;
		ov_callbacks	vorbisCb;
		sOggFile		oggFile;
		ALenum			format = AL_FORMAT_MONO16;

		alGenBuffers(1, &iBuffer);

		pFileSystem->Open(filename, &stFile, pool);

		void *tmp = const_cast<void *>(stFile.GetData());
		oggFile.dataPtr = static_cast<u8 *>(tmp);
		oggFile.dataRead = 0;
		oggFile.dataSize = stFile.GetSize();

		vorbisCb.read_func = vorbis_read;
		vorbisCb.close_func = vorbis_close;
		vorbisCb.seek_func = vorbis_seek;
		vorbisCb.tell_func = vorbis_tell;

		if (ov_open_callbacks(&oggFile, &oggStream, NULL, 0, vorbisCb) != 0)
		{
			Log(TAG "Could not read ogg file from memory");
		
			memset(&oggFile, '\0', sizeof(oggFile));
			stFile.Close();
		}
		else
		{
			int bitstream = 0;
			long bytes;
			char arr[BUFFER_SIZE];

			ALsizei freq;
			std::vector<char> buffer;
			
			vorbis_info *info = ov_info(&oggStream, -1);

			if (info->channels > 1)
				format = AL_FORMAT_STEREO16;

			freq = info->rate;
			do
			{
				bytes = ov_read(&oggStream, arr, BUFFER_SIZE, 0, 2, 1, &bitstream);

				if (bytes < 0)
				{
					ov_clear(&oggStream);
					Log(TAG "Error decoding ogg stream.");
				}

				buffer.insert(buffer.end(), arr, arr + bytes);
			} while (bytes > 0);

			ov_clear(&oggStream);
			alBufferData(iBuffer, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);
		}

	#if !defined(DEBUG)
		stFile.Close();
	#endif

		this->bLoaded = TRUE;
	}

	return this->bLoaded;
}

INLINE BOOL Sound::Unload()
{
	alDeleteBuffers(1, &iBuffer);
	stFile.Close();
	this->bLoaded = FALSE;

	return TRUE;
}

INLINE const void *Sound::GetData() const
{
	return static_cast<const void *>(&iBuffer);
}

INLINE u32 Sound::GetUsedMemory() const
{
	return ISound::GetUsedMemory() + sizeof(this) + stFile.GetSize();
}


}} // namespace


#endif // _OAL_OGG_
