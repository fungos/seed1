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

/*! \file ParticleEmitter.cpp
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/

#include "ParticleEmitter.h"
#include "Rand.h"
#include "Number.h"
#include "Texture.h"

namespace Seed {

ParticleEmitter::ParticleEmitter()
	: pEmitterObject(NULL)
	, psInfo()
	, pRes(NULL)
	, pPool(NULL)
	, pFilename(NULL)
	, pSpriteFilename(NULL)
	, bParticlesFollowEmitter(FALSE)
	, fAge(0.0f)
	, fRespawnAge(0.0f)
	, fEmissionResidue(0.0f)
	, fInterval(0.0f)
	, ptPrevLocation(0.0f, 0.0f)
	, fTx(0.0f)
	, fTy(0.0f)
	, fScale(1.0f)
	, iAnimation(0)
	, bPaused(FALSE)
	, bEnabled(TRUE)
	, nMinFilter(TextureFilterLinear)
	, nMagFilter(TextureFilterLinear)
	, arParticles()
{
}

ParticleEmitter::~ParticleEmitter()
{
	this->Unload();
}

INLINE void ParticleEmitter::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	if (bEnabled)
	{
		ASSERT_NULL(filename);
		ASSERT_NULL(res);
		ASSERT_NULL(pool);

		pRes = res;
		pPool = pool;
		pFilename = filename;
		pEmitterObject = reinterpret_cast<ParticleEmitterObject *>(res->Get(filename, Seed::ObjectParticleEmitterObject, pool));
		psInfo = *(pEmitterObject->GetData());
		iAnimation = psInfo.iTextureFrame;
		fInterval = psInfo.fInterval;
	}
	//this->Stop();
}

INLINE void ParticleEmitter::Unload()
{
	for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Unload();
		arParticles[i].bActive = FALSE;
	}

	sRelease(pEmitterObject);

	pRes		= NULL;
	pPool		= NULL;
	pFilename	= NULL;
	pSpriteFilename = NULL;
}

INLINE void ParticleEmitter::Reset()
{
	ITransformable2D::Reset();
	//ptLocation.x = 0.0f;
	//ptLocation.y = 0.0f;
	ptPrevLocation.x = 0.0f;
	ptPrevLocation.y = 0.0f;
	fTx = 0.0f;
	fTy = 0.0f;
	fScale = 1.0f;
	fEmissionResidue = 0.0f;
	//iParticlesAlive = 0;
	fAge = -2.0f;
	fRespawnAge = 0.0f;
	bPaused = FALSE;

	for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Unload();
		arParticles[i].bActive = FALSE;
	}
}

INLINE void ParticleEmitter::Update(f32 deltaTime)
{
	if (!(bEnabled && pSpriteFilename && !bPaused))
		return;

	//deltaTime = 0.017000001f;

	//u32 i = 0;
	f32 ang = 0.0f;
	Particle *par = NULL;
	Point<f32> accel(0.0f, 0.0f);
	Point<f32> accel2(0.0f, 0.0f);
	Point<f32> location = this->GetPosition();

	if (fAge == -2.0f && psInfo.fLifetime != -1.0f && fInterval > 0.0f)// && nParticlesAlive == 0)
	{
		fRespawnAge += deltaTime;

		if (fRespawnAge >= fInterval)
		{
			fAge = 0.0f;
			fRespawnAge = 0.0f;
		}
		//return;
	}

	if (fAge >= 0)
	{
		fAge += deltaTime;
		if (fAge >= psInfo.fLifetime)
			fAge = -2.0f;
	}

	for (s32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		par = &arParticles[i];

		par->fAge += deltaTime;
		if (par->fAge >= par->fTerminalAge)
		{
			//par->SetVisible(FALSE);

			arParticles[i].Unload();
			arParticles[i].bActive = FALSE;

			//iParticlesAlive--;
			//MEMCOPY(par, &arParticles[iParticlesAlive], sizeof(Particle));
			//i--;
			continue;
		}

		accel = par->ptPos - location;
		accel.Normalize();
		accel2 = accel;
		accel *= par->fRadialAccel;

		// vecAccel2.Rotate(M_PI_2);
		// the following is faster
		ang = accel2.x;
		accel2.x = -accel2.y;
		accel2.y = ang * 1.25f;

		accel2 *= par->fTangentialAccel;
		par->ptVelocity += (accel + accel2) * deltaTime;
		par->ptVelocity.y += par->fGravity * deltaTime * 1.25f;

		par->fSpin += par->fSpinDelta * deltaTime;
		par->fSize += par->fSizeDelta * deltaTime;
		par->fColorR += par->fColorDeltaR * deltaTime;
		par->fColorG += par->fColorDeltaG * deltaTime;
		par->fColorB += par->fColorDeltaB * deltaTime;
		par->fColorA += par->fColorDeltaA * deltaTime;

		par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
		//par->SetScale(par->fSize);
		//par->AddPosition(par->ptVelocity * deltaTime);
		par->ptScale.x = par->fSize;
		par->ptScale.y = par->fSize;
		par->fRotation += par->fSpin;
		//if (par->fRotation >= 360.0f)
		//	par->fRotation -= 360.0f;
		//if (par->fRotation < 0.0f)
		//	par->fRotation += 360.0f;

		par->ptPos += (par->ptVelocity * deltaTime);
		bTransformationChanged = TRUE;
		par->bTransformationChanged = TRUE;
		par->bChanged = TRUE;
		//par++;
	}

	// generate new particles
	if (fAge != -2.0f)
	{
		f32 fParticlesNeeded = psInfo.iEmission * deltaTime + fEmissionResidue;
		u32 iParticlesCreated = static_cast<u32>(Number::Ceil(fParticlesNeeded));
		fEmissionResidue = fParticlesNeeded - iParticlesCreated;

		//par = &arParticles[iParticlesAlive];

		for (u32 i = 0; i < iParticlesCreated; i++)
		{
			//if (arParticles.Size() >= SEED_PARTICLES_MAX)
			//	break;

			BOOL bFull = TRUE;
			for (u32 j = 0; j < SEED_PARTICLES_MAX; j++)
			{
				if (!arParticles[j].bActive)
				{
					arParticles[j].bActive = TRUE;
					par = &arParticles[j];
					bFull = FALSE;
					break;
				}
			}
			if (bFull)
				break;

			par->fAge = 0.0f;
			par->fTerminalAge = pRand->Get(psInfo.fParticleLifeMin, psInfo.fParticleLifeMax) * 0.70f;

			Point<f32> pos = ptPrevLocation + (location - ptPrevLocation) * pRand->Get(0.0f, 1.0f);

			if (!psInfo.fWidth)
			{
				pos.x += pRand->Get(-0.0002f, 0.0002f);
			}
			else
			{
				pos.x += pRand->Get(psInfo.fWidth);
			}

			if (!psInfo.fHeight)
			{
				pos.y += pRand->Get(-0.0002f, 0.0002f);
			}
			else
			{
				pos.y += pRand->Get(psInfo.fHeight);
			}

			ang = psInfo.fDirection - kPiOver2 + pRand->Get(0, psInfo.fSpread) - psInfo.fSpread / 2.0f;

			if (psInfo.bRelative)
				ang += (ptPrevLocation - location).Angle() + kPiOver2;

			par->ptVelocity.x = cosf(ang);
			par->ptVelocity.y = sinf(ang) * 1.25f;
			par->ptVelocity *= pRand->Get(psInfo.fSpeedMin/300.0f, psInfo.fSpeedMax/300.0f) * 0.70f;

			par->fGravity = pRand->Get(psInfo.fGravityMin/900.0f, psInfo.fGravityMax/900.0f) * 3.0f;
			par->fRadialAccel = pRand->Get(psInfo.fRadialAccelMin/900.0f, psInfo.fRadialAccelMax/900.0f) * 4.0f;
			par->fTangentialAccel = pRand->Get(psInfo.fTangentialAccelMin/900.0f, psInfo.fTangentialAccelMax/900.0f) * 3.0f;

			par->fSize = pRand->Get(psInfo.fSizeStart, psInfo.fSizeStart + (psInfo.fSizeEnd - psInfo.fSizeStart) * psInfo.fSizeVar);
			par->fSizeDelta = (psInfo.fSizeEnd - par->fSize) / par->fTerminalAge;

			par->fSpin = pRand->Get(psInfo.fSpinStart, psInfo.fSpinStart + (psInfo.fSpinEnd - psInfo.fSpinStart) * psInfo.fSpinVar) / 15.0f;
			par->fSpinDelta = (psInfo.fSpinEnd - par->fSpin) / par->fTerminalAge;

			par->fColorR = pRand->Get(psInfo.fColorStartR, psInfo.fColorStartR + (psInfo.fColorEndR - psInfo.fColorStartR) * psInfo.fColorVar);
			par->fColorG = pRand->Get(psInfo.fColorStartG, psInfo.fColorStartG + (psInfo.fColorEndG - psInfo.fColorStartG) * psInfo.fColorVar);
			par->fColorB = pRand->Get(psInfo.fColorStartB, psInfo.fColorStartB + (psInfo.fColorEndB - psInfo.fColorStartB) * psInfo.fColorVar);
			par->fColorA = pRand->Get(psInfo.fColorStartA, psInfo.fColorStartA + (psInfo.fColorEndA - psInfo.fColorStartA) * psInfo.fAlphaVar);

			par->fColorDeltaR = (psInfo.fColorEndR - par->fColorR) / par->fTerminalAge;
			par->fColorDeltaG = (psInfo.fColorEndG - par->fColorG) / par->fTerminalAge;
			par->fColorDeltaB = (psInfo.fColorEndB - par->fColorB) / par->fTerminalAge;
			par->fColorDeltaA = (psInfo.fColorEndA - par->fColorA) / par->fTerminalAge;

			par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
			if (psInfo.iBlendMode == 6)
				par->SetBlending(BlendModulate);
			else
				par->SetBlending(BlendAdditive);
			//par->SetScale(par->fSize);
			par->ptScale.x = par->fSize;
			par->ptScale.y = par->fSize;
			//par->SetPosition(pos);
			par->ptPos = pos;
			//par->fRotation = par->fSpin;
			par->bTransformationChanged = TRUE;

			par->Load(pSpriteFilename, pRes, pPool);
			par->SetAnimation(iAnimation);
			par->SetVisible(TRUE);
			//par->SetParent(this);

			//iParticlesAlive++;
			//par++;
		}
	}

	ITexture *img = arParticles[0].GetTexture();
	if (img)
	{
		img->SetFilter(TextureFilterTypeMag, nMagFilter);
		img->SetFilter(TextureFilterTypeMin, nMinFilter);
	}

	if (bParticlesFollowEmitter)
		MoveEverything(ptPos);

	ptPrevLocation = location;
	bTransformationChanged = FALSE;

	for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Update(deltaTime);
	}
}

INLINE void ParticleEmitter::Render()
{
	if (bEnabled)
	{
		for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			arParticles[i].Render();
		}
	}
}

INLINE void ParticleEmitter::SetSprite(const char *filename)
{
	if (bEnabled)
	{
		pSpriteFilename = filename;
		for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			arParticles[i].Load(filename, pRes, pPool);
			arParticles[i].SetAnimation(iAnimation);
		}
	}
}

INLINE void ParticleEmitter::SetAnimation(u32 anim)
{
	iAnimation = anim;
	for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].SetAnimation(anim);
	}
}

INLINE void ParticleEmitter::Play()
{
	if (bEnabled)
	{
		fInterval = psInfo.fInterval;

		if (!bPaused)
		{
			ptPrevLocation = ptPos;
			if (psInfo.fLifetime == -1.0f)
				fAge = -1.0f;
			else
				fAge = 0.0f;

			fRespawnAge = 0.0f;
		}
		else
		{
			bPaused = FALSE;
		}
	}
}

INLINE BOOL ParticleEmitter::IsPlaying() const
{
	return !(this->IsStopped() || this->IsPaused());
}

INLINE void ParticleEmitter::Stop()
{
	fRespawnAge = 0.0f;
	fAge = -2.0f;
	fInterval = 0.0f;
	bPaused = FALSE;
}

INLINE BOOL ParticleEmitter::IsStopped() const
{
	return (fAge == -2.0f);
}

INLINE void ParticleEmitter::Pause()
{
	bPaused = TRUE;
}

INLINE BOOL ParticleEmitter::IsPaused() const
{
	return bPaused;
}

INLINE void ParticleEmitter::Kill()
{
	fRespawnAge = 0.0f;
	fAge = -2.0f;
//	iParticlesAlive = 0;
	bPaused = FALSE;

	for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Unload();
		arParticles[i].bActive = FALSE;
	}
}

INLINE void ParticleEmitter::Enable()
{
	bEnabled = TRUE;
}

INLINE void ParticleEmitter::Disable()
{
	bEnabled = FALSE;
}

INLINE BOOL ParticleEmitter::IsEnabled() const
{
	return bEnabled;
}

INLINE void ParticleEmitter::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin)
	{
		nMinFilter = filter;
	}
	else if (type == Seed::TextureFilterTypeMag)
	{
		nMagFilter = filter;
	}
}

INLINE void ParticleEmitter::MoveEverything(const Point<f32> &pos)
{
	Point<f32> dpos = pos - ptPrevLocation;
	ptPrevLocation = pos;

	for (u32 i = 0; i < SEED_PARTICLES_MAX; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].AddPosition(dpos);
	}
}

INLINE void ParticleEmitter::SetParticlesFollowEmitter(BOOL bFollow)
{
	this->bParticlesFollowEmitter = bFollow;
}

INLINE ParticleEmitterInfo *ParticleEmitter::GetEmitterInfo()
{
	return &this->psInfo;
}

INLINE const char *ParticleEmitter::GetObjectName() const
{
	return "ParticleEmitter";
}

INLINE int ParticleEmitter::GetObjectType() const
{
	return Seed::ObjectParticleEmitter;
}

} // namespace
