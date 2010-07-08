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

/*! \file ParticleEmitter.h
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__

#include "interface/ITransformable2D.h"
#include "ParticleEmitterObject.h"

namespace Seed {

class ITexture;
class ResourceManager;
class IMemoryPool;

class SEED_CORE_API ParticleEmitter : public ISceneObject
{
	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();

		virtual void SetSprite(const char *filename);
		virtual void SetAnimation(u32 anim);

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		virtual void Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
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
		virtual void Render();

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
