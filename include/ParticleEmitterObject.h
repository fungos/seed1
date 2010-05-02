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

struct SEED_CORE_API ParticleEmitterInfo
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

class SEED_CORE_API ParticleEmitterObject : public IResource
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
		// IResource
		virtual BOOL Unload();
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);

		SEED_DISABLE_INSTANCING;

	private:
		SEED_DISABLE_COPY(ParticleEmitterObject);

	private:
		//Data	sInfo;
		File	stFile;
		u32		iMemory;
};

} // namespace

#endif // __PARTICLE_EMITTER_OBJECT_H__
