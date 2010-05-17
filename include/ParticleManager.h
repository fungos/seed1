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

/*! \file ParticleManager.h
	\author	Danny Angelo Carminati Grein
	\brief PArticle emitter manager
*/

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "Array.h"
#include "Singleton.h"

#define MAX_PARTICLE_EMITTERS	128

namespace Seed {

class ParticleEmitter;

class SEED_CORE_API ParticleManager : public IModule, public IUpdatable
{
	SEED_SINGLETON_DECLARE(ParticleManager);

	public:
		virtual void Play();
		virtual BOOL IsPlaying() const;

		virtual void Stop();
		virtual BOOL IsStopped() const;

		virtual void Pause();
		virtual BOOL IsPaused() const;

		virtual void Kill();

		virtual void Add(ParticleEmitter *emitter);
		virtual void Remove(ParticleEmitter *emitter);

		virtual void Simulate(u32 iNumLoops);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual void Disable();
		virtual void Enable();

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(ParticleManager);

	private:
		Array<ParticleEmitter *, MAX_PARTICLE_EMITTERS>	vEmitter;

		BOOL	bPaused;
		BOOL	bStopped;
};

//extern "C" {
//SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(ParticleManager);
//}

#define pParticleManager ParticleManager::GetInstance()

} // namespace

#endif // __PARTICLE_MANAGER_H__
