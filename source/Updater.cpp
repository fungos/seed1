/*! \file Updater.cpp
	\author	Danny Angelo Carminati Grein
	\brief Module updater
*/

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
	while(fAccumulator >= resolution)
	{
		for(u32 i = 0; i < len; i++)
		{
			arUpdatable[i]->Update(resolution);
		}

		fAccumulator -= resolution;
	}

	/*f32 alpha = fAccumulator / resolution;
	for(u32 i = 0; i < len; i++)
	{
		arUpdatable[i]->Update(resolution * alpha);
	}
	fAccumulator = 0.0f;*/
}

} // namespace
