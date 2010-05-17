/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file ModuleManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Module manager
*/

#include "ModuleManager.h"
#include "interface/IModule.h"

#define TAG "[ModuleManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(ModuleManager);

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
