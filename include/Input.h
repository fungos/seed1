/*! \file Input.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __INPUT_H__
#define __INPUT_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiInput.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/sdl/SdlInput.h"
	using namespace Seed::SDL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphInput.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#if defined(_QT_)
	#include "platform/qt/QtInput.h"
	using namespace Seed::QT;
#endif // _QT_


#endif // __INPUT_H__
