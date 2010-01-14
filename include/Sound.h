/*! \file Sound.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __SOUND_H__
#define __SOUND_H__

#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiSound.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/oal_ogg/OalOggSound.h"
	using namespace Seed::OAL;
#endif // _SDL_ || _QT_

#ifdef _IPHONE_
	#include "platform/iphone/IphSound.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __SOUND_H__
