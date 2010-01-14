/*! \file FileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__


#include "Config.h"

#if defined(_WII_)
	#include "platform/wii/WiiFileSystem.h"
	using namespace Seed::WII;
	#define FSNS WII
#endif // _WII_

#if defined(_SDL_) || defined(_QT_) || defined(_IPHONE_)
	#include "platform/pc/PcFileSystem.h"
	using namespace Seed::PC;
	#define FSNS PC
#endif // _SDL_ || _QT_ || _IPHONE_


#endif // __FILE_SYSTEM_H__
