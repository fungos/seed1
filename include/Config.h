/*! \file Config.h
	\author	Danny Angelo Carminati Grein
	\brief Engine configuration header
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

// FIXME: Xcode or cmake isnt setting these defines correcly - quick hack to get it working on mac
#if defined(__APPLE_CC__)
#define DEBUG
#define _SDL_
//#define YMEM_DEBUG
#endif


#define DANNY	"Danny Angelo Carminati Grein"
#define PATUTI	"Everton Fernando Patitucci da Silva"
#define RAPTOR	"Rafael Eduardo Gonchor"

#define SEED_TAG					"[Seed] "
#define SEED_VERSION_MINOR			1			// Developer version (features, fixes)
#define SEED_VERSION_MIDDLE			0			// Client version
#define SEED_VERSION_MAJOR			0			// Release version (final trunk)
#define SEED_VERSION_STRING			"0.0.1"
#define SEED_NAME					"Seed SDK"
#define SEED_COPYRIGHT				"Copyright (c) 2008-2009 Danny Angelo Carminati Grein\nCopyright (c) 2009 TechFront Studios"
#define SEED_AUTHORS				"Authors:\n\t" DANNY "\n\t" PATUTI "\n\t" RAPTOR


#ifdef DEBUG
	#define SEED_TYPE "Debug"
#else
	#define SEED_TYPE "Release"
#endif // DEBUG

#if defined(_IPHONE_)
	#define SEED_PLATFORM_NAME "iPhone"
#elif defined(_WII_)
	#define SEED_PLATFORM_NAME "Nintendo Wii"
#elif defined(_SDL_)
	#define SEED_PLATFORM_NAME "SDL"
#elif defined(_QT_)
	#define SEED_PLATFORM_NAME "Qt"
#endif // _IPHONE_

#define SEED_MESSAGE		SEED_NAME " " SEED_VERSION_STRING " [" SEED_PLATFORM_NAME " " SEED_TYPE "]\n" SEED_COPYRIGHT "\n" SEED_AUTHORS


//#include "AutoConfig.h"

//================================================================================
// FEATURES CAPPING
//================================================================================
#define SEED_USE_THEORA						1
#define SEED_USE_3D							0
#define SEED_USE_UNSTABLE					0
#define SEED_ENABLE_DEPTH_TEST				0
//#define SEED_USE_REAL_COORDINATE_SYSTEM		1

/*
During load, upload texture to vga and keep it there.
*/
#define SEED_ENABLE_PRELOAD_TEXTURE			1

/*
Keep a local ram copy of the image uploaded to vram, this is used in SDL and iPhone
only. On SDL so that we can see memory consumption, and on iPhone and SDL so we can
use Alpha test per-pixel.
*/
#define SEED_ENABLE_KEEP_IMAGE_DATA			0

/*
If the Seed will be added to the game project.
*/
#define SEED_BUILTIN						0

/*
The size of the music buffer for streaming from hard disk on platforms that support it (SDL).
*/
#define SEED_MUSIC_STREAMING_BUFFER_SIZE	(1024 * 40)

/*
Maximum amount of particles per emitter.
*/
#define SEED_PARTICLES_MAX					500

/*
String Pools - Define the amount of each pool
You MUST fine tune this accordingly your target platform, your needs and your use.
Try to keep these values at a minimum.
*/
#define SEED_USE_STRING_POOL				1
#define SEED_STRING_POOL_32					128000		// 4Kb
#define SEED_STRING_POOL_64					128		// 8Kb
#define SEED_STRING_POOL_128				128		// 16kb
#define SEED_STRING_POOL_256				64		// 16kb
#define SEED_STRING_POOL_512				64		// 32kb
#define SEED_STRING_POOL_1024				32		// 32kb
#define SEED_STRING_POOL_2048				32		// 64kb
#define SEED_STRING_POOL_4096				32		// 128kb - 300kb total sum + little more

//================================================================================
// FEATURES SPECIFIC SETTINGS
//================================================================================


//================================================================================
// DEBUGGING DEFINES
//================================================================================
#ifdef DEBUG
	#define DEBUG_GAMESTATE_NOWAIT			1	// To disable save system sync delay
	#define DEBUG_ENABLE_RECT_TEXTAREA		1	// Enable TextArea Wireframe debugging
	#define DEBUG_ENABLE_RECT_LABEL			1	// Enable Label Wireframe debugging
	#define DEBUG_ENABLE_RECT_BUTTON		1	// Enable Button Wireframe debugging
	#define DEBUG_ENABLE_RECT_SPRITE		1	// Enable Sprite Wireframe debugging

	#define DEBUG_RECT_COLOR_TEXTAREA		PIXEL_COLOR(255, 0, 0, 255)
	#define DEBUG_RECT_COLOR_LABEL			PIXEL_COLOR(0, 255, 0, 255)
	#define DEBUG_RECT_COLOR_BUTTON			PIXEL_COLOR(0, 0, 255, 255)
	#define DEBUG_RECT_COLOR_SPRITE			PIXEL_COLOR(255, 255, 255, 255)

	#define SEED_LOG_RESOURCEMANAGER		1
#endif // DEBUG


#if SEED_BUILTIN == 1
	#define INLINE inline
// __attribute__((always_inline)) // GCC
#else
	#define INLINE
#endif // SEED_BUILTIN

#if SEED_ENABLE_KEEP_IMAGE_DATA == 1 && defined(RELEASE)
#warning "SEED_ENABLE_KEEP_IMAGE_DATA is on!"
#endif // SEED_ENABLE_KEEP_IMAGE_DATA

#endif // __CONFIG_H__
