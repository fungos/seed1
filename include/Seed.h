/*! \file Seed.h
	\author	Danny Angelo Carminati Grein
	\brief Seed SDK
*/

#ifndef __SEED_H__
#define __SEED_H__

#include <Config.h>
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

#include <Input.h>
#include <Image.h>

#include <interface/IEventInputMotionListener.h>
#include <interface/IEventInputKeyboardListener.h>
#include <interface/IEventInputPointerListener.h>
#include <interface/IEventResourceLoaderListener.h>
#include <interface/IEventSystemListener.h>
#include <interface/IEventFileSystemListener.h>
#include <interface/IEventMovieListener.h>
#include <interface/IObject.h>

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
#include <Renderer2D.h>

#include <SaveSystem.h>
#include <Screen.h>
#include <Sound.h>
#include <SoundSystem.h>
#include <StateMachine.h>
#include <Sprite.h>
#include <StringPool.h>
#include <System.h>
#include <Updater.h>

#include <Text.h>
#include <Thread.h>
#include <Timer.h>
#include <Trigonometry.h>
#include <TString.h>

#include <Timeline.h>
#include <Movie.h>

#include <SeedInit.h>

#if SEED_USE_THEORA == 1
#include <api/theora/Theora.h>
#endif

#ifdef _WII_
	#ifdef USE_AILIVE
		#include <platform/wii/WiiAiLive.h>
	#endif // USE_AILIVE
#endif // _WII_

#endif // __SEED_H__
