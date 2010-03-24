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
#include "interface/IRenderer.h"

/// Seed SDK base namespace
/**
Everything from Seed SDK will be inside Seed namespace.
*/
namespace Seed {

	extern ResourceManager glResourceManager;

	#if SEED_USE_STRING_POOL == 1
	extern StringPoolManager<u16> glStringPool;
	#endif // SEED_USE_STRING_POOL

	/// Set the Game Application main class.
	/**
 	When creating a new Game project, you must have a class that will use IGameApp as a base class. This class will be the main
	entrypoint to your game and must be set by usign this function.

	\param app A pointer to your game application main class.
	\param argc Command line argument count.
	\param argv A pointer to the command line arguments.
 	*/
	void SetGameApp(IGameApp *app, int argc, char **argv);

	/// Set which IRenderer to use.
	/**
 	You must instantiate a IRenderer to use inside your Setup method, and then pass a pointer to Seed. This will be deprecated
	anytime soon.

	\param renderer A pointer to the IRenderer that you instantiated.
 	*/
	void SetRenderer(IRenderer *renderer);

	/// Displays a output message by using IGameApp::WriteOut implementation.
	/**
 	This method is a channeling point to all output messages that Seed will use. If you do not override IGameApp::WriteOut, it
	will use the default stdout output pipe.

	\param msg Pointer to a string to be displayed.
 	*/
	void WriteOut(const char *msg);

	/// Displays a error message by using IGameApp::WriteErr implementation.
	/**
 	This method is a channeling point to all error messages that Seed will use. If you do not override IGameApp::WriteErr, it
	will use the default stderr output pipe.
 	*/
	void WriteErr(const char *msg);

	/// Initialize Seed
	/**
 	This will initialize all Seed states and subsystems and must be called before anything and after Seed::SetGameApp.
 	*/
	BOOL Initialize();

	/// Update Seed
	/**
 	Will update all Seed states and subsystems and will be called once per tick until a shutdown or error happens.
 	*/
	void Update();

	/// Render
	/**
 	Renders the game state and will be called once per frame.
 	*/
	void Render(f32 delta);

	/// Shutdown Seed
	/**
 	Will reset all Seed states and shutdown all subsystems. Nothing more from Seed can be used until a new Initialize happens.
 	*/
	void Shutdown();


	/// Private states that Seed maintain, must not be accessed directly.
	namespace Private
	{
		/// A pointer to the IGameApp implementation.
		extern IGameApp	*pApplication;

		/// A pointer to the user IRenderer so Seed can query for informations and do operations with it.
		extern IRenderer	*pRenderer;

		/// Flag indicating that Seed is Initialized or not.
		extern BOOL		bInitialized;

		/// Command line argument count.
		extern int		iArgc;
		
		/// Command line command string.
		extern char		**pcArgv;

		/// Last frame time
		extern f32 fCurrentTime;
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


#endif // __SEED_INIT_H__
