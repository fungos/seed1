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

/*! \file SeedInit.h
	\author	Danny Angelo Carminati Grein
	\brief Initialization / Shutdown
*/

#ifndef __SEED_INIT_H__
#define __SEED_INIT_H__

#include "ResourceManager.h"
#include "StringPool.h"
#include "interface/IGameApp.h"
#include "interface/IInput.h"
#include "interface/IRendererDevice.h"
#include "Timer.h"

extern "C" {

/// Seed SDK base namespace
/**
Everything from Seed SDK will be inside Seed namespace.
*/
namespace Seed {

	SEED_CORE_API extern ResourceManager *pResourceManager;
	SEED_CORE_API extern const Configuration *pConfiguration;

	#if SEED_USE_STRING_POOL == 1
	SEED_CORE_API extern StringPoolManager<u16> glStringPool;
	#endif // SEED_USE_STRING_POOL

	/// Set the Game Application main class.
	/**
	When creating a new Game project, you must have a class that will use IGameApp as a base class. This class will be the main
	entrypoint to your game and must be set by usign this function.

	\param app A pointer to your game application main class.
	\param argc Command line argument count.
	\param argv A pointer to the command line arguments.
	*/
	SEED_CORE_API void SetGameApp(IGameApp *app, int argc, char **argv);

	/// Displays a output message by using IGameApp::WriteOut implementation.
	/**
	This method is a channeling point to all output messages that Seed will use. If you do not override IGameApp::WriteOut, it
	will use the default stdout output pipe.

	\param msg Pointer to a string to be displayed.
	*/
	SEED_CORE_API void WriteOut(const char *msg);

	/// Displays a error message by using IGameApp::WriteErr implementation.
	/**
	This method is a channeling point to all error messages that Seed will use. If you do not override IGameApp::WriteErr, it
	will use the default stderr output pipe.
	*/
	SEED_CORE_API void WriteErr(const char *msg);

	/// Initialize Seed
	/**
	This will initialize all Seed states and subsystems and must be called before anything and after Seed::SetGameApp.
	*/
	SEED_CORE_API BOOL Initialize();

	/// Update Seed
	/**
	Will update all Seed states and subsystems and will be called once per tick until a shutdown or error happens.
	*/
	SEED_CORE_API void Update();

	/// Render
	/**
 	Renders the game state and will be called once per frame.
 	*/
	SEED_CORE_API void Render();

	/// Shutdown Seed
	/**
	Will reset all Seed states and shutdown all subsystems. Nothing more from Seed can be used until a new Initialize happens.
	*/
	SEED_CORE_API void Shutdown();

	/// Get the version of the linked Seed
	/**
	Get the linked library version number, you can pass NULL to ignore any parameter.
	*/
	SEED_CORE_API void GetVersion(u32 *major, u32 *middle, u32 *minor);

	/// Private states that Seed maintain, must not be accessed directly.
	namespace Private
	{
		/// A pointer to the IGameApp implementation.
		SEED_CORE_API extern IGameApp	*pApplication;

		/// Flag indicating that Seed is Initialized or not.
		SEED_CORE_API extern BOOL		bInitialized;

		/// Command line argument count.
		SEED_CORE_API extern int		iArgc;

		/// Command line command string.
		SEED_CORE_API extern char		**pcArgv;

		/// Last frame time
		SEED_CORE_API extern f32 fCurrentTime;
	} // namespace

	/// iPhone platform related implementation using iPhoneOS SDK
	namespace iPhone
	{
	} // namespace

	/// SDL (Win32, Win64, Linux, MacOSX) platform related implementation
	namespace SDL
	{
	} // namespace

	/// Revolution platform related implementation using RVL_SDK
	namespace WII
	{
	} // namespace

	/// Qt platform related implementation
	namespace QT
	{
	} // namespace
} // namespace

} // extern C

#endif // __SEED_INIT_H__
