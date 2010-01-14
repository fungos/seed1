/*! \file System.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiSystem.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlSystem.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphSystem.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtSystem.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __SYSTEM_H__
