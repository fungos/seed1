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

/*! \file ParticleManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Particle emitter manager
*/

#include "ParticleManager.h"
#include "ParticleEmitter.h"

namespace Seed {

SEED_SINGLETON_DEFINE(ParticleManager);

ParticleManager::ParticleManager()
	: vEmitter()
	, bPaused(FALSE)
	, bStopped(FALSE)
{
	vEmitter.Truncate();
}

ParticleManager::~ParticleManager()
{
	this->Reset();
}

INLINE BOOL ParticleManager::Initialize()
{
	IModule::Initialize();

	return TRUE;
}

INLINE BOOL ParticleManager::Reset()
{
	this->Kill();
	vEmitter.Truncate();

	return TRUE; // abstract IModule::Reset();
}

INLINE BOOL ParticleManager::Shutdown()
{
	this->Reset();
	return IModule::Shutdown();;
}

INLINE void ParticleManager::Simulate(u32 iNumLoops)
{
	for (u32 i = 0; i < iNumLoops; i++)
	{
		this->Update(0.01f);
	}
}

INLINE BOOL ParticleManager::Update(f32 dt)
{
	UNUSED(dt);
	if (bEnabled && !(bPaused || bStopped))
	{
		for (u32 i = 0; i < vEmitter.Size(); i++)
		{
			ParticleEmitter *p = vEmitter[i];
			ASSERT_NULL(p);

			if (!p->IsPaused())
				p->Update(dt);
		}
	}

	return TRUE;
}

INLINE void ParticleManager::Kill()
{
	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		ParticleEmitter *p = vEmitter[i];
		ASSERT_NULL(p);

		p->Kill();
	}
}

INLINE void ParticleManager::Stop()
{
	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		ParticleEmitter *p = vEmitter[i];
		ASSERT_NULL(p);

		p->Stop();
	}

	bPaused = FALSE;
	bStopped = TRUE;
}

INLINE BOOL ParticleManager::IsStopped() const
{
	return bStopped;
}

INLINE void ParticleManager::Pause()
{
	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		ParticleEmitter *p = vEmitter[i];
		ASSERT_NULL(p);

		p->Pause();
	}

	bPaused = TRUE;
}

INLINE BOOL ParticleManager::IsPaused() const
{
	return bPaused;
}

INLINE void ParticleManager::Play()
{
	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		ParticleEmitter *p = vEmitter[i];
		ASSERT_NULL(p);

		p->Play();
	}

	bPaused = FALSE;
	bStopped = FALSE;
}

INLINE BOOL ParticleManager::IsPlaying() const
{
	return !(bPaused || bStopped);
}

INLINE void ParticleManager::Disable()
{
	IModule::Disable();

	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		ParticleEmitter *p = vEmitter[i];
		ASSERT_NULL(p);

		p->Disable();
	}
}

INLINE void ParticleManager::Enable()
{
	IModule::Enable();

	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		ParticleEmitter *p = vEmitter[i];
		ASSERT_NULL(p);

		p->Enable();
	}
}

INLINE void ParticleManager::Add(ParticleEmitter *emitter)
{
	ASSERT_NULL(emitter);

	BOOL found = FALSE;
	for (u32 i = 0; i < vEmitter.Size(); i++)
	{
		if (vEmitter[i] == emitter)
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		vEmitter.Add();
		vEmitter[vEmitter.Size() - 1] = emitter;
	}
}

INLINE void ParticleManager::Remove(ParticleEmitter *emitter)
{
	ASSERT_NULL(emitter);
	vEmitter.Remove(emitter);
}

INLINE const char *ParticleManager::GetObjectName() const
{
	return "ParticleEmitter";
}

INLINE int ParticleManager::GetObjectType() const
{
	return Seed::ObjectParticleManager;
}

} // namespace
