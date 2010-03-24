/*! \file ParticleManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Particle emitter manager
*/

#include "ParticleManager.h"
#include "ParticleEmitter.h"

namespace Seed {

ParticleManager ParticleManager::instance;

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

	return IModule::Reset();
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
