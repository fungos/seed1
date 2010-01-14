/*! \file Mutex.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __MUTEX_H__
#define __MUTEX_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiMutex.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlMutex.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphMutex.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtMutex.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __MUTEX_H__
