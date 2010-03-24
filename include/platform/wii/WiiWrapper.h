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

#ifndef __WIIWRAPPERSTUB_H__
#define __WIIWRAPPERSTUB_H__

#if defined(_WII_)
	#include "WiiWrapperRVL.h"
#else
#define WII_MAX_PATH				0
#define WII_MAX_CONTROLLERS			0
#define WII_MAX_READ_BUFS			0
#define WII_MAX_VOICES				0
#define GetDSPADPCMDataAddress(a)	0

#define WiiFileInfo			void
#define WiiHandle			void
#define WiiDataInfo			void
#define WiiControllerData	void
#define WiiRenderModeObj	void
#define WiiTick				void
#define WiiPalettePtr		void
#define WiiTexObj			void
#define WiiInputStatus		void
#define WiiHeapHandle		void
#define WiiAllocator		void
#define WiiVoiceData		void
#define WiiMutex			void
#define WiiFifoObj			void
#define WiiRenderModeObj	void
#define WiiMixer			void
#define WiiVolume			void
#define WiiLoop				void
#define WiiVec				void
#define WiiMtx				void
#define WiiThread			void

#define WiiGetTick
#define WiiPanic
#define WiiHalt
#define WiiRoundUp32B
#define WiiAllocFromAllocator
#define WiiFreeToAllocator
#define WiiGetTotalFreeSizeForExpHeap
#endif

#endif // __WIIWRAPPERSTUB_H__
