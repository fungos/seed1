/*! \file Thread.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __THREAD_H__
#define __THREAD_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiThread.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlThread.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphThread.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtThread.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __THREAD_H__
