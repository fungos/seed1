/*! \file ModuleManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Module manager
*/

#include "ModuleManager.h"
#include "interface/IModule.h"

#define TAG "[ModuleManager] "

namespace Seed {

ModuleManager ModuleManager::instance;

ModuleManager::ModuleManager()
	: arModule()
{
}

ModuleManager::~ModuleManager()
{
	arModule.Truncate();
}

BOOL ModuleManager::Add(IModule *obj)
{
	arModule.Add(obj);
	BOOL ret = obj->Initialize();

	if (!ret)
	{
		if (obj->IsRequired())
		{
			Info(TAG "CRITICAL: Module '%s' failed to initialize.", obj->GetObjectName());
		}
		else
		{
			Info(TAG "WARNING: Module '%s' failed to initalize.", obj->GetObjectName());
			ret = TRUE; // we can continue as this module isn't critical.
		}
	}

	return ret;
}

BOOL ModuleManager::Remove(IModule *obj)
{
	arModule.Remove(obj);
	return obj->Shutdown();
}

void ModuleManager::Disable(const char *moduleName)
{
	UNUSED(moduleName);
}

void ModuleManager::Enable(const char *moduleName)
{
	UNUSED(moduleName);
}

BOOL ModuleManager::IsEnabled(const char *moduleName)
{
	UNUSED(moduleName);
	return TRUE;
}

BOOL ModuleManager::Initialize()
{
	BOOL ret = TRUE;

	u32 len = arModule.Size();
	for (u32 i = 0; i < len; i++)
	{
		IModule *obj = arModule[i];
		ret = ret && (obj->Initialize() || !obj->IsRequired());
	}

	return ret;
}

BOOL ModuleManager::Reset()
{
	BOOL ret = TRUE;

	u32 len = arModule.Size();
	for (u32 i = 0; i < len; i++)
	{
		IModule *obj = arModule[i];
		ret = ret && (obj->Reset() || !obj->IsRequired());
	}

	return ret;
}

BOOL ModuleManager::Shutdown()
{
	BOOL ret = TRUE;

	u32 len = arModule.Size() - 1;
	for (u32 i = len; i > 0; --i)
	{
		IModule *obj = arModule[i];
		ret = ret && (obj->Shutdown() || !obj->IsRequired());
	}

	return ret;
}

INLINE void ModuleManager::Print()
{
	Info(TAG "Listing current modules:");
	u32 len = arModule.Size();
	for (u32 i = 0; i < len; i++)
	{
		IModule *obj = arModule[i];
		Info(TAG "\tModule: %s.", obj->GetObjectName());
	}
}

} // namespace
