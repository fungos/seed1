/*! \file Renderer.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __RENDERER_H__
#define __RENDERER_H__


#include "Config.h"

#if defined(_WII_)
	#include "platform/wii/WiiRenderer.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_) || defined(_QT_)
	#include "api/ogl/OglRenderer.h"
	using namespace Seed::OGL;
#endif // _SDL_

#if defined(_IPHONE_)
	#include "platform/iphone/IphRenderer.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_


#endif // __RENDERER_H__
