/*! \file Cartridge.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __CARTRIDGE_H__
#define __CARTRIDGE_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiCartridge.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_) || defined(_IPHONE_)
	#include "platform/pc/PcCartridge.h"
	using namespace Seed::PC;
#endif // _SDL_


#endif // __CARTRIDGE_H__
