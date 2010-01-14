/*! \file Particle.cpp
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/


#include "Particle.h"
#include "Enum.h"


namespace Seed {


Particle::Particle() 
	: bActive(FALSE)
	, ptVelocity(0.0f, 0.0f)
	, fGravity(0.0f)
	, fRadialAccel(0.0f)
	, fTangentialAccel(0.0f)
	, fSpin(0.0f)
	, fSpinDelta(0.0f)
	, fSize(0.0f)
	, fSizeDelta(0.0f)
	, fAge(0.0f)
	, fTerminalAge(0.0f)
	, fColorR(0.0f)
	, fColorG(0.0f)
	, fColorB(0.0f)
	, fColorA(0.0f)
	, fColorDeltaR(0.0f)
	, fColorDeltaG(0.0f)
	, fColorDeltaB(0.0f)
	, fColorDeltaA(0.0f)
{
}

Particle::~Particle()
{
}

INLINE const char *Particle::GetObjectName() const
{
	return "Particle";
}

INLINE int Particle::GetObjectType() const
{
	return Seed::ObjectParticle;
}


} // namespace
