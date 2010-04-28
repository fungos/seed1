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

/*! \file WiiSoundSource.h
	\author	Danny Angelo Carminati Grein
	\brief Sound source implementation for Wii platform using AX
*/

#ifndef __WII_SOUND_SOURCE_H__
#define __WII_SOUND_SOURCE_H__

#if defined(_WII_)

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
