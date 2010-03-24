/*! \file IUpdatable.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines an updatable class
*/

#include "interface/IUpdatable.h"
#include "Log.h"

namespace Seed {

IUpdatable::IUpdatable()
{
}

IUpdatable::~IUpdatable()
{
}

INLINE BOOL IUpdatable::Update(f32 dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;
	return TRUE;
}

} // namespace


