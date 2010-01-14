/*! \file Timer.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __TIMER_H__
#define __TIMER_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiTimer.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlTimer.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphTimer.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtTimer.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __TIMER_H__
