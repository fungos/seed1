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

/*! \file WiiSoundHelper.h
	\author	Danny Angelo Carminati Grein
	\brief Helper to SoundSystem functionality
*/

#ifndef __WII_SOUND_HELPER_H__
#define __WII_SOUND_HELPER_H__

#if defined(_WII_)

#include "Defines.h"

namespace Seed { namespace WII {

#define Bytes2Nibbles(n) 	(n << 1)
#define AUDIO_FRAME_TIME 	3
#define MAX_MUSIC_VOLUME 	0x5000
#define MAX_SFX_VOLUME 		0x8000
#define DEFAULT_FADE_OUT	15		//FIXME

// All sounds are played at a volume of 1.0 (0x8000).
extern WiiMixer g_mix;
extern WiiMixer g_Lmix;
extern WiiMixer g_Rmix;
extern WiiVolume g_ve;

struct sAdpcmInfo
{
	BOOL 			bHasLoop;
	WiiLoop			loopInfo;

	sAdpcmInfo() : bHasLoop(FALSE), loopInfo() {}
};

BOOL load_sound_adpcm(const void *pData, WiiVoiceData *&pVoice, WiiMixer *pMix, BOOL setLoop);//sAdpcmInfo *info);

}} // namespace

#else // _WII_
	#error "Do not include 'platform/wii/WiiSoundHelper.h'"
#endif // _WII_
#endif // __WII_SOUND_HELPER_H__
