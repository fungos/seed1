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
