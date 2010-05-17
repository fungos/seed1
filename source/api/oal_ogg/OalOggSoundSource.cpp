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

/*! \file OalOggSoundSource.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sound source implementation using OpenAL API
*/

#include "SoundSource.h"

#if defined(_OAL_OGG_)

#include "SoundSystem.h"
#include "Sound.h"
#include "Log.h"
#include "File.h"
#include "Formats.h"

#define TAG		"[SoundSource] "

namespace Seed { namespace OAL {

SoundSource::SoundSource()
	: iSource(0)
	, pSound(NULL)
	, stFile()
{
}

SoundSource::~SoundSource()
{
	this->Unload();
}

void SoundSource::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);
	ASSERT_NULL(res);

	if (pSoundSystem->IsInitialized())
	{
		this->Unload();

		/* Open file .sound */
		SECURITY_CHECK(pFileSystem->Open(filename, &stFile, pool), "Sound object couldn't be opened");

		const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
		ObjectHeader *block = NULL;
		READ_STRUCT(block, ObjectHeader, ptr);
		SECURITY_CHECK(seed_validate_block(&stFile, block, SOUND_OBJECT_MAGIC, SOUND_OBJECT_VERSION), "Invalid block header for sound.");

		u32 volume = 0;
		READ_U32(volume, ptr);
		this->fVolume  = volume / 100.0f;

		u32 flags = 0;
		READ_U32(flags, ptr);
		this->bLoop = ((flags & 0x01) == 0x01); // FIXME

		const char *fname = NULL;
		READ_STR(fname, ptr);
		ASSERT_NULL(fname);

		/* Get the resource */
		pSound = static_cast<Sound *>(res->Get(fname, Seed::ObjectSound, pool));

		if (iSource)
			alDeleteSources(1, &iSource);
		ALenum err = alGetError();

		alGenSources(1, &iSource);
		err = alGetError();
		if (err != AL_NO_ERROR)
			Info(TAG "Could not create OpenAL Source: %4x", err);

		const ALint *buffer = static_cast<const ALint *>(pSound->GetData());

		alSourcef(iSource, AL_PITCH, 1.0f);
		alSource3f(iSource, AL_POSITION, cPosition.x, cPosition.y, cPosition.z);
		alSource3f(iSource, AL_VELOCITY, cVelocity.x, cVelocity.y, cVelocity.z);
		alSourcei(iSource, AL_LOOPING, this->bLoop);
		alSourcei(iSource, AL_BUFFER, *buffer);
		this->SetVolume(this->fVolume);
	}
}

INLINE void SoundSource::Unload()
{
	if (Private::bInitialized)
		pSoundSystem->Remove(this);

	if (iSource)
		alDeleteSources(1, &iSource);

	sRelease(pSound);

	stFile.Close();
}

INLINE void SoundSource::SetVolume(f32 vol)
{
	ISoundSource::SetVolume(vol);
	alSourcef(iSource, AL_GAIN, this->fVolume * pSoundSystem->GetSfxVolume());
}

INLINE void SoundSource::UpdateVolume()
{
	alSourcef(iSource, AL_GAIN, this->fVolume * pSoundSystem->GetSfxVolume());
}

INLINE void SoundSource::SetLoop(BOOL b)
{
	ISoundSource::SetLoop(b);
	alSourcei(iSource, AL_LOOPING, b);
}

INLINE void SoundSource::Stop(f32 ms)
{
	UNUSED(ms)

	alSourceStop(this->iSource);
	eState = Seed::SourceStop;
}

INLINE void SoundSource::Play()
{
	alSourceStop(this->iSource);
	alSourcePlay(this->iSource);
	eState = Seed::SourcePlaying;
}

INLINE void SoundSource::Resume()
{
	if (eState == SourcePause || eState == SourcePaused)
	{
		alSourcePlay(this->iSource);
		eState = SourcePlay;
	}
}

}} // namespace

#endif // _OAL_OGG_
