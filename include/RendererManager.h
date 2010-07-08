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

/*! \file RendererManager.h
	\author	Everton Fernando Patitucci da Silva
	\brief Renderer Manager
*/

#ifndef __RENDERER_MANAGER_H__
#define __RENDERER_MANAGER_H__

#include "Array.h"
#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "Singleton.h"

namespace Seed {

class Renderer;

class SEED_CORE_API RendererManager : public IModule, public IUpdatable
{
	SEED_SINGLETON_DECLARE(RendererManager);
	public:
		virtual void Add(Renderer *renderer);
		virtual void Remove(Renderer *renderer);

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		virtual void Disable();
		virtual void Enable();

		// IUpdatable
		virtual BOOL Update(f32 dt);

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(RendererManager);

		Array<Renderer *, SEED_RENDERER_MAX> arRenderer;
		BOOL bEnabled;
};

#define pRendererManager RendererManager::GetInstance()

} // namespace

#endif // __RENDERER_MANAGER_H__
