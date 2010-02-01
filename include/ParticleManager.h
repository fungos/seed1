/*! \file ParticleManager.h
	\author	Danny Angelo Carminati Grein
	\brief PArticle emitter manager
*/

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__


#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "Array.h"


#define MAX_PARTICLE_EMITTERS	128

namespace Seed {

class ParticleEmitter;

class ParticleManager : public IModule, public IUpdatable
{
	public:
		static ParticleManager instance;

	public:
		ParticleManager();
		virtual ~ParticleManager();

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
		//typedef std::vector<ParticleEmitter *> EmitterVector;
		//typedef EmitterVector::iterator EmitterIterator;
		//EmitterVector vEmitter;

		Array<ParticleEmitter *, MAX_PARTICLE_EMITTERS>	vEmitter;

		BOOL	bPaused;
		BOOL	bStopped;
};


ParticleManager *const pParticleManager = &ParticleManager::instance;


} // namespace


#endif // __PARTICLE_MANAGER_H__

