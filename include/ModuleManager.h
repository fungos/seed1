/*! \file ModuleManager.h
	\author	Danny Angelo Carminati Grein
	\brief Module Manager
*/

#ifndef __MODULE_MANAGER_H__
#define __MODULE_MANAGER_H__

#include "Array.h"
#define MAX_MODULES 32

namespace Seed {

class IModule;

class ModuleManager
{
	public:
		ModuleManager();
		~ModuleManager();

		BOOL Add(IModule *obj);
		BOOL Remove(IModule *obj);

		void Disable(const char *moduleName);
		void Enable(const char *moduleName);
		BOOL IsEnabled(const char *moduleName);

		BOOL Initialize();
		BOOL Reset();
		BOOL Shutdown();

		void Print();

	public:
		static ModuleManager instance;

	private:
		SEED_DISABLE_COPY(ModuleManager);

	private:
		Array<IModule *, MAX_MODULES> arModule;
};

ModuleManager *const pModuleManager = &ModuleManager::instance;

} // namespace

#endif // __MODULE_MANAGER_H__
