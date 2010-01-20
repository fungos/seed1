/*! \file Updater.cpp
	\author	Danny Angelo Carminati Grein
	\brief Module updater
*/

#include <math.h>
#include "Updater.h"

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
	fAccumulator += dt;

	u32 len = arUpdatable.Size();
	do
	{
		for(u32 i = 0; i < len; i++)
		{
			arUpdatable[i]->Update(resolution);
		}

		fAccumulator -= resolution;
	}
	while(fAccumulator >= resolution);
}

} // namespace
