/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file IphSoundSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Log.h"

#include "MemoryManager.h"
#include "FileSystem.h"
#include "SoundSystem.h"
#include "System.h"
#include "Formats.h"
#include "IphoneView.h"
#include "SeedInit.h"

#include <stdio.h>
#include <algorithm>

#include <Foundation/Foundation.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AVFoundation/AVFoundation.h>
#include <Foundation/NSURL.h>
#include <Foundation/NSBundle.h>

#define TAG "[SoundSystem] "

#define AUDIO_FRAME_TIME 3
#define AUDIO_DATA_PATH		"/" FILESYSTEM_DEFAULT_PATH
#define AUDIO_DATA_EXT		".caf"


typedef ALvoid AL_APIENTRY (*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid *data, ALsizei size, ALsizei freq);
ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid *data, ALsizei size, ALsizei freq)
{
	static alBufferDataStaticProcPtr proc = NULL;

	if (proc == NULL)
	{
		proc = (alBufferDataStaticProcPtr)alcGetProcAddress(NULL, (const ALCchar *)"alBufferDataStatic");
	}

	if (proc)
		proc(bid, format, data, size, freq);
}

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(SoundSystem);

IResource *AudioResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);
	pSoundSystem->Load(filename, pool);
	return pSoundSystem;
}

SoundSystem::SoundSystem()
	: pAVPlayer(NULL)
	, bLoaded(FALSE)
	, pData(NULL)
	, pFilename(NULL)
	//, musVoice()
	, fVolume(IPHONE_MASTER_VOLUME)
	, fOriginalVolume(IPHONE_MASTER_VOLUME)
	, fElapsedTime(0.0f)
	, iFadeFrames(0)
{
	MEMSET(sndVoice, '\0', sizeof(sndVoice));
	MEMSET(sndFile, '\0', sizeof(sndFile));
	MEMSET(musFile, '\0', sizeof(musFile));

	this->pData = NULL;

	this->Reset();
}

SoundSystem::~SoundSystem()
{
	this->Reset();
}

BOOL SoundSystem::Initialize()
{
	Log(TAG "Initializing...");

	this->bLoaded = FALSE;

	ALenum error = alGetError();
	ALCdevice *device = alcOpenDevice(NULL);
	error = alGetError();
	ALCcontext *context = alcCreateContext(device, NULL);
	error = alGetError();
	alcMakeContextCurrent(context);
	error = alGetError();

	alListener3f(AL_POSITION, 0.0f, 0.0f, 1.0f);
	error = alGetError(); // ??????

	BOOL r = this->Reset();

	Log(TAG "Initialization completed. Free Memory: %d.", pMemoryManager->GetFreeMemory());

	return r;
}

BOOL SoundSystem::Reset()
{
	if (this->bLoaded)
	{
		pMemoryManager->Free(this->pData);
		this->pFilename			= NULL;
		this->fOriginalVolume	= IPHONE_MASTER_VOLUME;
		this->fVolume			= IPHONE_MASTER_VOLUME;
		this->bLoaded			= FALSE;
		this->fElapsedTime		= 0.0f;

		// Free sounds
		for (u32 i = 0; i < IPHONE_MAX_VOICES; i++)
		{
			if (alIsSource(sndVoice[i].iSource))
			{
				alSourceStop(sndVoice[i].iSource);
				alDeleteSources(1, &sndVoice[i].iSource);
			}

			sndVoice[i].iState = SOUND_STATE_STOPPED;
			sndVoice[i].iSource = 0;
			sndVoice[i].fVolume = 0;
		}

		for (u32 i = 0; i < IPHONE_MAX_FILES; i++)
		{
			if (sndFile[i].pData)
			{
				pMemoryManager->Free(sndFile[i].pData);
				sndFile[i].pData = NULL;
				sndFile[i].iBuffer = 0;
			}

			sndFile[i].iSize = 0;
			sndFile[i].eFormat = 0;
			sndFile[i].iFreq = 0;
		}

		/*
		// Free musics
		if (alIsSource(musVoice.iSource))
		{
			alSourceStop(musVoice.iSource);
			alDeleteSources(1, &musVoice.iSource);
		}
		musVoice.iState = SOUND_STATE_STOPPED;
		musVoice.iSource = 0;
		musVoice.fVolume = 0;

		for (u32 i = 0; i < IPHONE_MAX_FILES; i++)
		{
			if (musFile[i].pData)
			{
				pMemoryManager->Free(musFile[i].pData);
				musFile[i].pData = NULL;
				musFile[i].iBuffer = 0;
			}

			musFile[i].iSize = 0;
			musFile[i].eFormat = 0;
			musFile[i].iFreq = 0;
		}
		*/
	}

	MEMSET(sndVoice, '\0', sizeof(sndVoice));
	MEMSET(sndFile, '\0', sizeof(sndFile));
	//MEMSET(musFile, '\0', sizeof(musFile));

	return TRUE;
}

BOOL SoundSystem::Shutdown()
{
	Log(TAG "Terminating...");

	BOOL r = this->Reset();

	ALCcontext *context = NULL;
	ALCdevice  *device = NULL;

	context = alcGetCurrentContext();
	device = alcGetContextsDevice(context);

	alcDestroyContext(context);
	alcCloseDevice(device);

	Log(TAG "Terminated.");

	return r;
}

BOOL SoundSystem::Update(f32 delta)
{
	UNUSED(delta);

	this->UpdateSounds();
	this->UpdateMusic();

	return TRUE;
}

BOOL SoundSystem::Prepare(void *workBuffer, u32 bufferLength)
{
	UNUSED(workBuffer)
	UNUSED(bufferLength)

	return TRUE;
}

/*
BOOL SoundSystem::LoadPackage(const char *filename)
{
	glResourceManager.Get(filename, IObject::AUDIO, pLargePool);

	return TRUE;
}
*/

void SoundSystem::Load(const char *filename, IMemoryPool *pool)
{
	//return FALSE;
	ASSERT_NULL(filename);

	if (pFileSystem->Open(filename, &stFile, pLargePool))
	{
		this->pFilename = filename;
		u8 *ptr = static_cast<u8 *>(pData);

		ObjectHeader *oh;
		READ_STRUCT(oh, ObjectHeader, ptr);
		if (oh->magic != AUDIO_OBJECT_MAGIC)
		{
			Log(TAG "Invalid audio file %s.", filename);
			pMemoryManager->Free(this->pData);
			this->pData = NULL;
			this->pFilename = NULL;
			this->bLoaded = FALSE;

			ASSERT(0);
		}
		else if (oh->version != AUDIO_OBJECT_VERSION)
		{
			Log(TAG "Audio %s version %x required.", filename, AUDIO_OBJECT_VERSION);
			pMemoryManager->Free(this->pData);

			this->pData = NULL;
			this->pFilename = NULL;
			this->bLoaded = FALSE;

			ASSERT(0);
		}
		else if (oh->platform != PLATFORM_CODE)
		{
			Log(TAG "%s invalid platform code.", filename);

			pMemoryManager->Free(this->pData);
			this->pData = NULL;
			this->pFilename = NULL;
			this->bLoaded = FALSE;

			ASSERT(0);
		}
		else
		{
			AudioObjectHeader *aoh;
			READ_STRUCT(aoh, AudioObjectHeader, ptr);

			char *str;

			for (u32 i = 0; i < aoh->music_count; i++)
			{
				READ_STR(str, ptr);
				Log(TAG "File: %s", str);
				musFile[i] = str;
				//this->ReadData(&musFile[i], str);
			}

			for (u32 i = 0; i < aoh->sound_count; i++)
			{
				READ_STR(str, ptr);
				this->ReadData(&sndFile[i], str);
			}

			this->bLoaded = TRUE;
		}
	}
}

void SoundSystem::PlaySound(u32 soundId)
{
	if (!bLoaded)
		return;

	for (u32 i=0; i<IPHONE_MAX_VOICES; i++)
	{
		if (sndVoice[i].iState == SOUND_STATE_STOPPED)
		{
			alGenSources(1, &sndVoice[i].iSource);

			ALenum err = alGetError();
			if (err != AL_NO_ERROR)
			{
				Info(TAG "alGenSources error %d.", err);
				break;
			}

			alSource3f(sndVoice[i].iSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
			alSourcei(sndVoice[i].iSource, AL_LOOPING, 0);
			alSourcei(sndVoice[i].iSource, AL_BUFFER, sndFile[soundId].iBuffer);

			sndVoice[i].iState 	= SOUND_STATE_START;
			sndVoice[i].iId		= soundId;

			break;
		}
	}
}

void SoundSystem::StopSound(u32 soundId, u16 fadeFrames)
{
	UNUSED(fadeFrames)

	if (!bLoaded)
		return;

	for (u32 i = 0; i < IPHONE_MAX_VOICES; i++)
	{
		if (sndVoice[i].iId == soundId)
		{
			sndVoice[i].iState = SOUND_STATE_STOP;
		}
	}
}

void SoundSystem::StopSounds()
{
	for (u32 i = 0; i < IPHONE_MAX_FILES; i++)
		this->StopSound(i, 0);
}

void SoundSystem::SetSoundVolume(f32 volume)
{
}

void SoundSystem::UpdateSounds()
{
	if (!bLoaded)
		return;

	for (u32 i = 0; i < IPHONE_MAX_VOICES; i++)
	{
		if (sndVoice[i].iState == SOUND_STATE_STOPPED)
			continue;

		switch (sndVoice[i].iState)
		{
			case SOUND_STATE_START:
				sndVoice[i].iState = SOUND_STATE_STARTED;
			break;

			case SOUND_STATE_STARTED:
				alSourcePlay(sndVoice[i].iSource);
				sndVoice[i].iState = SOUND_STATE_PLAYING;
			break;

			case SOUND_STATE_PLAYING:
				ALint state;
				alGetSourcei(sndVoice[i].iSource, AL_SOURCE_STATE, &state);

				if (state == AL_STOPPED)
					sndVoice[i].iState = SOUND_STATE_STOPPED;
			break;

			case SOUND_STATE_STOP:
				alSourceStop(sndVoice[i].iSource);
				alDeleteSources(1, &sndVoice[i].iSource);
				sndVoice[i].iState = SOUND_STATE_STOPPED;
			break;

			default:
			break;
		}
	}
}

void SoundSystem::PlayMusic(u32 streamId)
{
	if (!bLoaded)
		return;

	if (this->iCurrentStream == streamId)
		return;

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;

	this->StopMusic(0);

	this->iCurrentStream = streamId;

	NSString *root = [NSString stringWithCString: iphGetRootPath() encoding: [NSString defaultCStringEncoding]];
	NSString *musicName = [NSString stringWithCString: musFile[streamId] encoding: [NSString defaultCStringEncoding]];
	NSString *extensionName = [NSString stringWithCString: ".mp3" encoding: [NSString defaultCStringEncoding]];
	NSString *dataPath = [@"/data/" stringByAppendingString: [musicName stringByAppendingString: extensionName]];
	NSString *path = [root stringByAppendingString: dataPath];
	NSError *err = NULL;
	p = [[[ AVAudioPlayer alloc ] initWithContentsOfURL: [ NSURL fileURLWithPath: path ] error: &err ] retain ];
	pAVPlayer = (void *)p;

	if (!err)
	{
		p.numberOfLoops = -1; // inf
		p.volume = this->fVolume;
		[p prepareToPlay];
		[p play];
	}
	else
	{
		Log(TAG "Error happened when trying to play music %d [%s]", streamId, musFile[streamId]);
	}
}

void SoundSystem::StopMusic(u16 fadeFrames)
{
	UNUSED(fadeFrames);

	if (!bLoaded)
		return;

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
	if (p)
	{
		[p stop];
		[p release];
		pAVPlayer = NULL;
		this->iCurrentStream = 999;
	}
}

void SoundSystem::PauseMusic()
{
	if (!bLoaded)
		return;

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
	if (p)
		[p pause];
}

void SoundSystem::ResumeMusic()
{
	if (!bLoaded)
		return;

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
	if (p)
	{
		p.volume = this->fVolume;
		[p play];
	}
}

void SoundSystem::UpdateMusic()
{
}

void SoundSystem::Mute()
{
}

void SoundSystem::Unmute()
{
}

void SoundSystem::SetMusicVolume(f32 volume)
{
	ASSERT_MSG((volume >= 0 || volume <= 1.0f), "Music volume must be between 0 and 1");

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;

	this->fVolume = volume;
	p.volume = volume;
}

void SoundSystem::ReadData(sSoundFileInfo *obj, const char *file)
{
	OSStatus					err = noErr;
	UInt64                      fileDataSize = 0;
	AudioStreamBasicDescription theFileFormat;
	UInt32                      thePropertySize = sizeof(theFileFormat);
	AudioFileID                 afid = 0;
	void						*theData = NULL;

	NSString *root = [NSString stringWithCString: iphGetRootPath() encoding: [NSString defaultCStringEncoding]];
	NSString *fname = [NSString stringWithCString: file encoding: [NSString defaultCStringEncoding]];
	NSString *ext = [NSString stringWithCString: AUDIO_DATA_EXT encoding: [NSString defaultCStringEncoding]];

	NSString *path = [fname stringByAppendingString: ext];
	path = [@AUDIO_DATA_PATH stringByAppendingString: path];
	path = [root stringByAppendingString: path];

	CFURLRef fileURL = CFURLCreateWithFileSystemPath(NULL, (CFStringRef)path, kCFURLPOSIXPathStyle, FALSE);
	err = AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &afid);
	if (err)
		{ Log(TAG "ReadAudioData: file %s%s%s not found, Error = %ld.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT, err); goto Exit; }

	err = AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &thePropertySize, &theFileFormat);
	if (err)
		{ Log(TAG "ReadAudioData: AudioFileGetProperty(kAudioFileProperty_DataFormat) FAILED, Error = %ld, File: %s%s%s.", err, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT); goto Exit; }

	if (theFileFormat.mChannelsPerFrame > 2)
	{
		Log(TAG "ReadAudioData - Unsupported Format, channel count is greater than stereo, File: %s%s%s.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		goto Exit;
	}

	if ((theFileFormat.mFormatID != kAudioFormatLinearPCM) || (!TestAudioFormatNativeEndian(theFileFormat)))
	{
		Log(TAG "ReadAudioData - Unsupported Format, must be little-endian PCM, File: %s%s%s.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		goto Exit;
	}

	if ((theFileFormat.mBitsPerChannel != 8) && (theFileFormat.mBitsPerChannel != 16))
	{
		Log(TAG "ReadAudioData - Unsupported Format, must be 8 or 16 bit PCM, File: %s%s%s.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		goto Exit;
	}

	thePropertySize = sizeof(fileDataSize);
	err = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize);

	if (err)
		{ Log(TAG "ReadAudioData: AudioFileGetProperty(kAudioFilePropertyAudioDataByteCount) FAILED, Error = %ld, File: %s%s%s.", err, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT); goto Exit; }

	UInt32 dataSize = static_cast<UInt32>(fileDataSize);
	theData = pMemoryManager->Alloc(dataSize);
	if (theData)
	{
		AudioFileReadBytes(afid, false, 0, &dataSize, theData);

		if (err == noErr)
		{
			obj->iSize = (ALsizei)dataSize;

			if (theFileFormat.mBitsPerChannel == 8)
				obj->eFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
			else
				obj->eFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

			obj->iFreq = (ALsizei)theFileFormat.mSampleRate;
			obj->pData = theData;

			obj->pFilename = file;

			ALenum error = alGetError();

			alGenBuffers(1, &obj->iBuffer);

			error = alGetError();

			alBufferDataStaticProc(obj->iBuffer, obj->eFormat, theData, obj->iSize, obj->iFreq);

			error = alGetError();

			if (error != AL_NO_ERROR)
			{
				Log(TAG "alBufferDataStaticProc buffer %d : error %d, File: %s%s%s.", obj->iBuffer, error, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);

				pMemoryManager->Free(theData);
				theData = NULL;
			}
		}
		else
		{
			pMemoryManager->Free(theData);
			theData = NULL;

			Log(TAG "ReadAudioData: ExtAudioFileRead FAILED, Error = %ld, File: %s%s%s.", err, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		}

	}

Exit:
	if (afid)
		AudioFileClose(afid);
}

}} // namespace

#endif // _IPHONE_
