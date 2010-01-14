/*! \file SoundSource.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __SOUND_SOURCE_H__
#define __SOUND_SOURCE_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiSoundSource.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/oal_ogg/OalOggSoundSource.h"
	using namespace Seed::OAL;
#endif // _SDL_ || _QT_

#ifdef _IPHONE_
	#include "platform/iphone/IphSoundSource.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __SOUND_SOURCE_H__
