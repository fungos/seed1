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

/*! \file IphSoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief SoundSystem Iphone Implementation
*/

#ifndef __IPH_SOUND_SYSTEM_H__
#define __IPH_SOUND_SYSTEM_H__

#if defined(_IPHONE_)

#include "interface/ISoundSystem.h"
#include "interface/IResource.h"
#include "File.h"
#include "Singleton.h"

#include <vector>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define IPHONE_MAX_VOICES		32
#define IPHONE_MAX_FILES		64
#define IPHONE_MASTER_VOLUME	0.2f

namespace Seed { namespace iPhone {

class SoundSystem : public ISoundSystem
{
	SEED_SINGLETON_DECLARE(SoundSystem)
	public:
		// ISoundSystem
		//virtual void PlayMusic(IMusic *mus, f32 ms = 0);
		//virtual void StopMusic(f32 ms = 0, IMusic *mus = NULL);
		//virtual void StopSounds();
		virtual void Pause();
		virtual void Resume();

		//virtual void Add(ISoundSource *src);
		//virtual void Remove(ISoundSource *src);

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	private:
		SEED_DISABLE_COPY(SoundSystem);

		void UpdateMusic(f32 dt, IMusic *mus);
		void UpdateSounds(f32 dt);

	private:
		ALCdevice			*pDevice;
		ALCcontext			*pContext;
};

#define pSoundSystem SoundSystem::GetInstance()

}} // namespace

#else // _IPHONE_
	#error "Include 'SoundSystem.h' instead 'platform/iphone/IphSoundSystem.h' directly."
#endif // _IPHONE_
#endif // __IHP_SOUND_SYSTEM_H__
