/*! \file Rand.cpp
	\author	Danny Angelo Carminati Grein
	\brief Pseudo random number generator
*/


#include "Rand.h"
#include <math.h>


namespace Seed {


Rand Rand::instance;


Rand::Rand()
	: iSeed(0)
{
}

Rand::~Rand()
{
}

INLINE void Rand::Initialize(u32 seed)
{
	if (!seed)
		iSeed = RAND;
	else
		iSeed = seed;
}

INLINE u32 Rand::Get(u32 max)
{
	return this->Get(0, max);
}

INLINE f32 Rand::Get(f32 max)
{
	return this->Get(0, max);
}

INLINE u32 Rand::Get(u32 min, u32 max)
{

	iSeed = 214013 * iSeed + 2531011;
	return min + (iSeed ^ iSeed >> 15) % (max - min);

	//return min + (RAND >> 15) % (max - min);
}

INLINE f32 Rand::Get(f32 min, f32 max)
{

	iSeed = 214013 * iSeed + 2531011;
	return min + (iSeed >> 16) * (1.0f / 65535.0f) * (max - min);

	//return min + (RAND >> 16) * (1.0f / 65535.0f) * (max - min);
}


} // namespace

