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

/*! \file SceneManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Scene Manager
*/

#include "SceneManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "interface/ISceneObject.h"

#define TAG		"[SceneManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(SceneManager);

SceneManager::SceneManager()
	: arObject()
{
	arObject.Truncate();
}

SceneManager::~SceneManager()
{
	arObject.Truncate();
}

void SceneManager::Reset()
{
	arObject.Truncate();
}

void SceneManager::Add(ISceneObject *obj)
{
	ASSERT_NULL(obj);

	BOOL found = FALSE;
	for (u32 i = 0; i < arObject.Size(); i++)
	{
		if (arObject[i] == obj)
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		arObject.Add(obj);
	}
}

void SceneManager::Remove(ISceneObject *obj)
{
	ASSERT_NULL(obj);
	arObject.Remove(obj);
}

INLINE BOOL SceneManager::Update(f32 delta)
{
	u32 len = arObject.Size();
	for (u32 i = 0; i < len; i++)
	{
		arObject[i]->Update(delta);
	}

	return TRUE;
}

} // namespace
