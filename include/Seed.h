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

/*! \file Seed.h
	\author	Danny Angelo Carminati Grein
	\brief Seed SDK
*/

#ifndef __SEED_H__
#define __SEED_H__

#include <Defines.h>
#include <MemoryManager.h>
#include <Enum.h>
#include <Vector.h>
#include <Log.h>
#include <Profiler.h>

#include <Array.h>

#include <Cartridge.h>
#include <Checksum.h>

#include <Dictionary.h>

#include <EventInputMotion.h>
#include <EventInputPointer.h>
#include <EventFileSystem.h>
#include <EventResourceLoader.h>
#include <EventSystem.h>
#include <EventMovie.h>

#include <File.h>
#include <FileSystem.h>
#include <Font.h>

#include <gui/Button.h>
#include <gui/EventWidget.h>
#include <gui/GuiManager.h>
#include <gui/Label.h>
#include <gui/TextArea.h>

#include <Image.h>
#include <Input.h>
#include <Texture.h>

#include <LeakReport.h>

#include <interface/IEventInputMotionListener.h>
#include <interface/IEventInputKeyboardListener.h>
#include <interface/IEventInputPointerListener.h>
#include <interface/IEventResourceLoaderListener.h>
#include <interface/IEventSystemListener.h>
#include <interface/IEventFileSystemListener.h>
#include <interface/IEventMovieListener.h>
#include <interface/IObject.h>
#include <interface/ISceneObject.h>
#include <interface/ISceneNode.h>

#include <Music.h>
#include <Mutex.h>

#include <Number.h>

#include <Package.h>
#include <PackageManager.h>
#include <ParticleEmitter.h>
#include <ParticleManager.h>
#include <Point.h>

#include <Rand.h>
#include <ResourceManager.h>
#include <ResourceLoader.h>
#include <ResourceGroup.h>
#include <Renderer.h>
#include <RendererDevice.h>

#include <SaveSystem.h>
#include <SceneNode.h>
#include <Screen.h>
#include <Sound.h>
#include <SoundSystem.h>
#include <StateMachine.h>
#include <Sprite.h>
#include <StringPool.h>
#include <SceneManager.h>
#include <System.h>
#include <Updater.h>

#include <Text.h>
#include <Thread.h>
#include <Timer.h>
#include <Trigonometry.h>
#include <TString.h>

#include <Timeline.h>
#include <Movie.h>

#include <Viewport.h>
#include <ViewManager.h>
#include <RendererManager.h>

#include <SeedInit.h>

#if SEED_USE_THEORA == 1
#include <api/theora/Theora.h>
#endif

#if defined(_WII_)
	#if defined(USE_AILIVE)
		#include <platform/wii/WiiAiLive.h>
	#endif // USE_AILIVE
#endif // _WII_

namespace Seed {};

#endif // __SEED_H__
