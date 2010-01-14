/*! \file MemoryManager.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__


#include "Config.h"
#include "Defines.h"

#if defined(_WII_)
	#include "platform/wii/WiiMemoryManager.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_) || defined(_IPHONE_)
	#include "platform/pc/PcMemoryManager.h"
	using namespace Seed::PC;
#endif // _SDL_ || _QT_ || _IPHONE_


#endif // __MEMORY_MANAGER_H__
