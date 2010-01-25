/*! \file Updater.cpp
	\author	Danny Angelo Carminati Grein
	\brief Module updater
*/

#include <math.h>
#include "Updater.h"
#include "interface/IUpdatable.h"

namespace Seed {

Updater Updater::instance;

Updater::Updater()
	: fAccumulator(0.0f)
	, arUpdatable()
{
}

Updater::~Updater()
{
	arUpdatable.Truncate();
}

void Updater::Add(IUpdatable *obj)
{
	arUpdatable.Add(obj);
}

void Updater::Remove(IUpdatable *obj)
{
	arUpdatable.Remove(obj);
}

void Updater::Run(f32 dt, f32 resolution)
{
	UNUSED(resolution)

	u32 len = arUpdatable.Size();
	for (u32 i = 0; i < len; i++)
		arUpdatable[i]->Update(dt);
}

} // namespace
