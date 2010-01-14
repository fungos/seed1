/*! \file Screen.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiScreen.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlScreen.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphScreen.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtScreen.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __SCREEN_H__
