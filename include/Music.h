/*! \file Music.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __MUSIC_H__
#define __MUSIC_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiMusic.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/oal_ogg/OalOggMusic.h"
	using namespace Seed::OAL;
#endif // _SDL_ || _QT_

#ifdef _IPHONE_
	#include "platform/iphone/IphMusic.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __MUSIC_H__
