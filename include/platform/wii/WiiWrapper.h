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
