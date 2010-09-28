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

/*! \file OalOggSound.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sound Implementation for OpenAL + Ogg
*/

#include "Sound.h"

#if defined(_IPHONE_)

#include "Log.h"

#undef WideChar
#include "platform/iphone/IphoneView.h"
#include <Foundation/Foundation.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AVFoundation/AVFoundation.h>
#include <Foundation/NSURL.h>
#include <Foundation/NSBundle.h>

#define TAG "[Sound] "

#define AUDIO_FRAME_TIME 3
#define AUDIO_DATA_PATH		"/" FILESYSTEM_DEFAULT_PATH "/"
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

IResource *SoundResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(res);

	Sound *sound = New(Sound());
	sound->Load(filename, res, pool);
	return sound;
}

Sound::Sound()
	: iBuffer(0)
	, iSize(0)
	, iFreq(0)
	, eFormat(0)
	, pData(NULL)
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
		pFilename = filename;
		pRes = res;
		pPool = pool;
		
		this->ReadData(filename);
		
		bLoaded = TRUE;
	}

	return bLoaded;
}

INLINE BOOL Sound::Unload()
{
	if (alIsBuffer(iBuffer))
		alDeleteBuffers(1, &iBuffer);
	
	if (pData)
		pMemoryManager->Free(pData, pPool);
		
	pData = NULL;
	eFormat = 0;
	iFreq = 0;
	iSize = 0;
	iBuffer = 0;
	
	bLoaded = FALSE;

	return TRUE;
}

INLINE const void *Sound::GetData() const
{
	return static_cast<const void *>(&iBuffer);
}

INLINE u32 Sound::GetUsedMemory() const
{
	return ISound::GetUsedMemory() + sizeof(this) + iSize;
}

void Sound::ReadData(const char *file)
{
	OSStatus					err = noErr;
	UInt64                      fileDataSize = 0;
	AudioStreamBasicDescription theFileFormat;
	UInt32                      thePropertySize = sizeof(theFileFormat);
	AudioFileID                 afid = 0;
	void						*theData = NULL;
	UInt32						dataSize = 0;

	NSString *root = [NSString stringWithCString: iphGetRootPath() encoding: [NSString defaultCStringEncoding]];
	NSString *fname = [NSString stringWithCString: file encoding: [NSString defaultCStringEncoding]];
	//NSString *ext = [NSString stringWithCString: AUDIO_DATA_EXT encoding: [NSString defaultCStringEncoding]];

	NSString *path = fname; //[fname stringByAppendingString: ext];
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

	dataSize = static_cast<UInt32>(fileDataSize);
	theData = pMemoryManager->Alloc(dataSize, pPool, "Sound Data", "Sound");
	if (theData)
	{
		AudioFileReadBytes(afid, false, 0, &dataSize, theData);

		if (err == noErr)
		{
			iSize = (ALsizei)dataSize;

			if (theFileFormat.mBitsPerChannel == 8)
				eFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
			else
				eFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

			iFreq = (ALsizei)theFileFormat.mSampleRate;
			pData = theData;

			ALenum error = alGetError();

			alGenBuffers(1, &iBuffer);

			error = alGetError();

			alBufferDataStaticProc(iBuffer, eFormat, pData, iSize, iFreq);

			error = alGetError();

			if (error != AL_NO_ERROR)
			{
				Log(TAG "alBufferDataStaticProc buffer %d : error %d, File: %s%s%s.", iBuffer, error, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);

				pMemoryManager->Free(theData, pPool);
				theData = NULL;
			}
		}
		else
		{
			pMemoryManager->Free(theData, pPool);
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
