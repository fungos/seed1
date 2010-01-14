/*! \file ParticleEmitterObject.h
	\author	Danny Angelo Carminati Grein
	\brief A particle emitter configuration file
*/

#ifndef __PARTICLE_EMITTER_OBJECT_H__
#define __PARTICLE_EMITTER_OBJECT_H__


#include "interface/IResource.h"
#include "Vector.h"
#include "Rect.h"
#include "Particle.h"
#include "SeedInit.h"
#include "File.h"


namespace Seed {


struct ParticleEmitterInfo
{
	u32					iDummy;
	u32					iTextureFrame;
	u32					iBlendMode;
	u32					iEmission; // particles per sec
	f32					fLifetime;

	f32					fParticleLifeMin;
	f32					fParticleLifeMax;

	f32					fDirection;
	f32					fSpread;
	BOOL				bRelative; // bool

	f32					fSpeedMin;
	f32					fSpeedMax;

	f32					fGravityMin;
	f32					fGravityMax;

	f32					fRadialAccelMin;
	f32					fRadialAccelMax;

	f32					fTangentialAccelMin;
	f32					fTangentialAccelMax;

	f32					fSizeStart;
	f32					fSizeEnd;
	f32					fSizeVar;

	f32					fSpinStart;
	f32					fSpinEnd;
	f32					fSpinVar;

	f32					fColorStartR;
	f32					fColorStartG;
	f32					fColorStartB;
	f32					fColorStartA;

	f32					fColorEndR;
	f32					fColorEndG;
	f32					fColorEndB;
	f32					fColorEndA;

	f32					fColorVar;
	f32					fAlphaVar;

	f32					fWidth;
	f32					fHeight;

	f32					fInterval;
};


IResource *ParticleEmitterObjectResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);


class ParticleEmitterObject : public IResource
{
	friend IResource *ParticleEmitterObjectResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class ParticleEmitter;

	public:
		ParticleEmitterObject();
		virtual ~ParticleEmitterObject();

		virtual const ParticleEmitterInfo *GetData() const;

		// IResource
		virtual u32 GetUsedMemory() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [](void *);

		// IResource
		virtual BOOL Unload();
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);

	private:
		SEED_DISABLE_COPY(ParticleEmitterObject);

	private:
		//Data	sInfo;
		File	stFile;
		u32		iMemory;
};


} // namespace


#endif // __PARTICLE_EMITTER_OBJECT_H__

