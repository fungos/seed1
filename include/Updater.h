/*! \file Updater.h
	\author	Danny Angelo Carminati Grein
	\brief Module updater
*/

#ifndef __UPDATER_H__
#define __UPDATER_H__

#include "interface/IUpdatable.h"
#include "Array.h"

namespace Seed {

#define MAX_UPDATEABLES 32

class Updater
{
	public:
		Updater();
		~Updater();

		void Run(f32 dt, f32 resolution);
		void Add(IUpdatable *obj);
		void Remove(IUpdatable *obj);

	public:
		static Updater instance;

	private:
		SEED_DISABLE_COPY(Updater);

	private:
		f32 fAccumulator;
		Array<IUpdatable*, MAX_UPDATEABLES> arUpdatable;
};

Updater *const pUpdater = &Updater::instance;

} // namespace

#endif // __UPDATER_H__
