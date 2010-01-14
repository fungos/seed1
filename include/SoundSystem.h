/*! \file SoundSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __SOUND_SYSTEM_H__
#define __SOUND_SYSTEM_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiSoundSystem.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/oal_ogg/OalOggSoundSystem.h"
	using namespace Seed::OAL;
#endif // _SDL_ || _QT_

#if defined(_IPHONE_)
	#include "platform/iphone/IphSoundSystem.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __SOUND_SYSTEM_H__
