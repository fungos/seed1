/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file RendererDevice.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __RENDERER_DEVICE_H__
#define __RENDERER_DEVICE_H__

#if defined(_WII_)
	#include "platform/wii/WiiRendererDevice.h"
	using namespace Seed::WII;
#endif // _WII_

#if defined(_SDL_)
	#include "platform/pc/PcRendererDevice.h"
	#include "api/ogl/Ogl14RendererDevice.h"

	#if defined(SEED_ENABLE_OGL20)
	#include "api/ogl/Ogl20RendererDevice.h"
	#endif

	#if defined(SEED_ENABLE_OGL30)
	#include "api/ogl/Ogl30RendererDevice.h"
	#endif

	#if defined(SEED_ENABLE_OGL40)
	#include "api/ogl/Ogl40RendererDevice.h"
	#endif

	#if defined(SEED_ENABLE_D3D8)
	#include "api/directx/D3D8RendererDevice.h"
	#endif

	#if defined(SEED_ENABLE_D3D9)
	#include "api/directx/D3D9RendererDevice.h"
	#endif

	#if defined(SEED_ENABLE_D3D10)
	#include "api/directx/D3D10RendererDevice.h"
	#endif

	#if defined(SEED_ENABLE_D3D11)
	#include "api/directx/D3D11RendererDevice.h"
	#endif

	using namespace Seed::PC;
#endif // _SDL_

#if defined(_QT_)
	#include "platform/qt/QtRendererDevice.h"
	using namespace Seed::QT;
#endif

#if defined(_IPHONE_)
	#include "platform/iphone/IphRendererDevice.h"
	using namespace Seed::iPhone;
#endif // _IPHONE_

#endif // __RENDERER_DEVICE_H__
