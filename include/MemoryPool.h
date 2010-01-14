/*! \file MemoryPool.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__


#include "Config.h"

/*
#if defined(_WII_)
	#include "platform/wii/WiiMemoryPool.h"
	using namespace Seed::WII;
#endif // _WII_
*/
#if defined(_SDL_) || defined(_QT_) || defined(_IPHONE_)
	#include "platform/pc/PcMemoryPool.h"
	using namespace Seed::PC;
#endif // _SDL_ || _QT_ || _IPHONE_

#endif // __MEMORY_POOL_H__
