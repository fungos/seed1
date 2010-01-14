/*! \file Particle.h
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/

#ifndef __PARTICLE_H__
#define __PARTICLE_H__


#include "Sprite.h"
#include "Point.h"


namespace Seed {


class Particle : public Sprite
{
	friend class ParticleEmitter;

	public:
		Particle();
		virtual ~Particle();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		BOOL	bActive;
		Point<f32>	ptVelocity;

		f32		fGravity;
		f32		fRadialAccel;
		f32		fTangentialAccel;

		f32		fSpin;
		f32		fSpinDelta;

		f32		fSize;
		f32		fSizeDelta;

		f32		fAge;
		f32		fTerminalAge;

		f32		fColorR;
		f32		fColorG;
		f32		fColorB;
		f32		fColorA;

		f32		fColorDeltaR;
		f32		fColorDeltaG;
		f32		fColorDeltaB;
		f32		fColorDeltaA;

		//ITransformable2D	*pObject;

	private:
		SEED_DISABLE_COPY(Particle);
};


} // namespace


#endif // __PARTICLE_H__

