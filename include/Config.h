/*! \file Config.h
	\author	Danny Angelo Carminati Grein
	\brief Engine configuration header
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#if defined(SEED_BUILD)
	#if !defined(SEED_BUILD_COMMERCIAL)
		#define SEED_BUILD_LGPL
		#define SEED_LICENSE "LGPL"
	#else
		#define SEED_LICENSE "Commercial"
	#endif // SEED_BUILD
#else
	#if !defined(SEED_USE_COMMERCIAL)
		#define SEED_USE_LGPL 1
		#define SEED_LICENSE "LGPL"
	#else
		#define SEED_LICENSE "Commercial"
	#endif
#endif

#define DANNY	"Danny Angelo Carminati Grein"
#define PATUTI	"Everton Fernando Patitucci da Silva"
#define RAPTOR	"Rafael Eduardo Gonchor"

#define SEED_TAG					"[Seed] "
#define SEED_VERSION_MINOR			4			// Developer version (features, fixes)
#define SEED_VERSION_MIDDLE			0			// Client version
#define SEED_VERSION_MAJOR			0			// Release version (final trunk)
#define SEED_VERSION_STRING			"%d.%d.%d"	//"0.0.3"
#define SEED_NAME					"Seed SDK"
#define SEED_COPYRIGHT				"Copyright (c) 2008-2009 Danny Angelo Carminati Grein\nCopyright (c) 2009 TechFront Studios"
#define SEED_AUTHORS				"Authors:\n\t" DANNY "\n\t" PATUTI "\n\t" RAPTOR

#define SEED_MESSAGE		SEED_NAME " " SEED_VERSION_STRING " [" SEED_PLATFORM_NAME " " SEED_TYPE " " SEED_LICENSE "]\n" SEED_COPYRIGHT "\n" SEED_AUTHORS

#if defined(DEBUG)
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

//================================================================================
// FEATURES CAPPING
//================================================================================
#define SEED_USE_THEORA						1
#define SEED_USE_3D							0
#define SEED_USE_UNSTABLE					0
#define SEED_ENABLE_DEPTH_TEST				0

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
Maxium amount of viewports.
*/
#define SEED_VIEWPORT_MAX	32

/*
Maximum amount of renderers.
*/
#define SEED_RENDERER_MAX	16

/*
Maximum amount of objects per scene node.
*/
#define SEED_SCENENODE_MAX	32

/*
Toggle between instancing Singleton classes in the (0) Stack or in the (1) Heap
*/
#define SEED_SINGLETON_HEAP					0

/*
*/
#define SEED_MAX_PRIORITY					1000000

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
#if defined(DEBUG)
	#define SEED_LOG_RESOURCEMANAGER		1
#endif // DEBUG

#if SEED_BUILTIN == 1
	#define INLINE inline
// __attribute__((always_inline)) // GCC
#else
	#define INLINE
#endif // SEED_BUILTIN

#endif // __CONFIG_H__
