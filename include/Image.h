/*! \file Image.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __IMAGE_H__
#define __IMAGE_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiImage.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlImage.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphImage.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtImage.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __IMAGE_H__
