/*! \file ParticleEmitter.h
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__

#include "interface/ITransformable2D.h"
#include "Point.h"
#include "Particle.h"
#include "ParticleEmitterObject.h"
#include "SeedInit.h"
#include "MemoryManager.h"
#include "Array.h"

namespace Seed {


class ParticleEmitter : public ITransformable2D
{
	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();

		virtual void SetSprite(const char *filename);
		virtual void SetAnimation(u32 anim);

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		virtual void Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual void Unload();
		virtual void Reset();

		virtual ParticleEmitterInfo *GetEmitterInfo();

		// Metodos candidatos a uma interface para sistemas Animados
		virtual void Play();
		virtual BOOL IsPlaying() const;

		virtual void Stop();
		virtual BOOL IsStopped() const;

		virtual void Pause();
		virtual BOOL IsPaused() const;

		virtual void Kill();

		virtual void Disable();
		virtual void Enable();
		virtual BOOL IsEnabled() const;

		virtual void SetParticlesFollowEmitter(BOOL bFollow);

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render(f32 delta);

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(ParticleEmitter);

		void MoveEverything(const Point<f32> &pos);

	private:
		ParticleEmitterObject		*pEmitterObject;
		ParticleEmitterInfo			psInfo;
		ResourceManager				*pRes;
		IMemoryPool					*pPool;
		const char					*pFilename;
		const char					*pSpriteFilename;
		BOOL						bParticlesFollowEmitter;

		f32							fAge;
		f32							fRespawnAge;
		f32							fEmissionResidue;
		f32							fInterval;

		Point<f32>					ptPrevLocation;
		//Point<f32>				ptLocation;
		f32							fTx;
		f32							fTy;
		f32							fScale;

		//u32							iParticlesAlive;
		u32							iAnimation;

		BOOL						bPaused;
		BOOL						bEnabled;

		eTextureFilter				nMinFilter;
		eTextureFilter				nMagFilter;

		Particle					arParticles[SEED_PARTICLES_MAX];
};


} // namespace


#endif // __PARTICLE_EMITTER_H__

