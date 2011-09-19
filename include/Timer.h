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

/*! \file Timer.h
	\author	Danny Angelo Carminati Grein
	\brief Include selector
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#if defined(_WII_)
	#include "platform/wii/WiiTimer.h"
	using namespace Seed::WII;
#elif defined(_SDL_)
	#include "platform/sdl/SdlTimer.h"
	using namespace Seed::SDL;
#elif defined(_IPHONE_)
	#include "platform/iphone/IphTimer.h"
	using namespace Seed::iPhone;
#elif defined(_QT_)
	#include "platform/qt/QtTimer.h"
	using namespace Seed::QT;
#elif defined(_PS3DEV_)
	#include "platform/ps3dev/PS3Timer.h"
	using namespace Seed::PS3;
#endif // platform

#endif // __TIMER_H__
