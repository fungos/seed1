/*! \file Sprite.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __SPRITE_H__
#define __SPRITE_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiSprite.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/ogl/OglSprite.h"
	using namespace Seed::OGL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphSprite.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __SPRITE_H__
