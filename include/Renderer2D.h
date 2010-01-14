/*! \file Renderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __RENDERER2D_H__
#define __RENDERER2D_H__


#include "Config.h"


#if defined(_WII_)
	#include "platform/wii/WiiRenderer2D.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/ogl/OglRenderer2D.h"
	using namespace Seed::OGL;
#endif // _SDL_ || _QT_

#if defined(_IPHONE_)
	#include "platform/iphone/IphRenderer2D.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __RENDERER2D_H__
