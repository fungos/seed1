/*! \file Rand.h
	\author	Danny Angelo Carminati Grein
	\brief Pseudo random number generator
*/

#ifndef __RAND_H__
#define __RAND_H__


#include "Defines.h"


namespace Seed {


class Rand
{
	public:
		Rand();
		virtual ~Rand();

		virtual void Initialize(u32 seed = 0);
		virtual u32 Get(u32 max);
		virtual f32 Get(f32 max);
		virtual u32 Get(u32 min, u32 max);
		virtual f32 Get(f32 min, f32 max);

	public:
		static Rand instance;

	private:
		SEED_DISABLE_COPY(Rand);

	private:
		u32 iSeed;
};

Rand *const pRand = &Rand::instance;


} // namespace



#endif // __RAND_H__
