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

/*! \file IGameApp.h
	\author	Danny Angelo Carminati Grein
	\brief The real main for a game application
*/

#ifndef __GAME_APP_H__
#define __GAME_APP_H__

#include "IUpdatable.h"
#include "IModule.h"
#include "Configuration.h"
#include "ResourceManager.h"

namespace Seed {

/// Game Application module
/**
Game application interface. This is responsible for pre-initialization setup (being deprecated soon) and Seed modules configurations.
You can reimplement WriteOut, WriteErr and WriteDbg to a better suited output of your platform.
*/
class SEED_CORE_API IGameApp : public IUpdatable, public IModule
{
	public:
		IGameApp();
		virtual ~IGameApp();

		virtual BOOL Shutdown();

		/// Setup must be called before Seed::Initialize - it will perform per-module configurations.
		/**
		\param argc Command line parameter count. Just pass what you receive in your main.
		\param argv Command line parameter values. Just pass what you receive in your main.
		*/
		virtual void Setup(int argc, char **argv);

		/// Print output level string
		virtual void WriteOut(const char *msg);

		/// Print error level string
		virtual void WriteErr(const char *msg);

		/// Print debug level string
		virtual void WriteDbg(const char *msg);

		/// Get user configuration
		const Configuration *GetConfiguration() const;

		/// Get user resource manager
		ResourceManager *GetResourceManager();

		/// Check whatever an system error ocurred. This is very important for multiplatform support (eg. errors can happend when reading disc within Nintendo Wii)
		virtual BOOL HasError() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		Configuration	cConfig;
		ResourceManager	cResourceManager;

	private:
		SEED_DISABLE_COPY(IGameApp);
};

} // namespace

#endif // __GAME_APP_H__
