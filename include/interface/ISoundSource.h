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

/*! \file ISoundSource.h
	\author	Danny Angelo Carminati Grein
	\brief A sound source interface to play 2D and 3D sounds
*/

#ifndef __ISOUND_SOURCE_H__
#define __ISOUND_SOURCE_H__

#include "Defines.h"
#include "Vector.h"
#include "interface/IObject.h"
#include "SeedInit.h"
#include "ResourceManager.h"

namespace Seed {

enum eSoundSourceState
{
	SourceNone,
	SourcePlay,
	SourcePlayStarted,
	SourcePlaying,
	SourceStop,
	SourceStopped,
	SourcePause,
	SourcePaused,
	SourceFadeIn,
	SourceFadingIn,
	SourceFadeOut,
	SourceFadingOut,
	SourceFading
};

class SEED_CORE_API ISoundSource : public IObject
{
	public:
		ISoundSource();
		virtual ~ISoundSource();
		
		//virtual void SetSound(ISound *sound);
		//virtual ISound *GetSound() const;
		virtual void Load(const char *filename, IMemoryPool *pool = pDefaultPool);
		virtual void Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual void Unload();

		virtual void SetPosition(f32 x, f32 y, f32 z);
		virtual void SetPosition(Vector3f vec);
		virtual void GetPosition(Vector3f *vec) const;

		virtual void SetVelocity(f32 x, f32 y, f32 z);
		virtual void SetVelocity(Vector3f vec);
		virtual void GetVelocity(Vector3f *vec) const;

		virtual void SetOrientation(f32 x, f32 y, f32 z);
		virtual void SetOrientation(Vector3f vec);
		virtual void GetOrientation(Vector3f *vec) const;
		
		virtual void SetVolume(f32 vol);
		virtual f32 GetVolume() const;
		/// When global volume is changed the SoundSystem will call UpdateVolume.
		virtual void UpdateVolume();
		
		virtual BOOL IsPlaying() const;
		virtual void Play();
		virtual void Stop(f32 ms = 0.0f);
		virtual void Pause();
		virtual void Resume();

		virtual void FadeOut(f32 ms);
		virtual void FadeIn(f32 ms);

		virtual void SetLoop(BOOL b);
		virtual BOOL IsLoop() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		virtual eSoundSourceState GetState() const;

	protected:
		Vector3f cPosition;
		Vector3f cVelocity;
		Vector3f cOrientation;
		f32 fMin;
		f32 fMax;
		f32 fVolume;
		f32 fFadeTime;
		u64 fStartFadeTime;
		eSoundSourceState eState;
		BOOL bLoop;

	private:
		SEED_DISABLE_COPY(ISoundSource);
};

} // namespace

#endif // __ISOUND_SOURCE_H__
