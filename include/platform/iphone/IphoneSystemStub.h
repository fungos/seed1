/*
 *  IphoneSystemStub.h
 *  BurgerIsland
 *
 *  Created by Danny Grein on 11/7/08.
 *  Copyright 2008 TechFront Studios. All rights reserved.
 *
 */

#ifndef _IPHONE_SYSTEM_STUB_H_
#define _IPHONE_SYSTEM_STUB_H_

#include "Defines.h"

struct iphTouchInfo
{
	u32 iTaps;
	f32 fPosX;
	f32 fPosY;
	f32 fRelX;
	f32 fRelY;
	f32 fOldX;
	f32 fOldY;
	u32 bStatus; // 1 press, 2 hold, 3 release, outro valor = invalido
};

extern iphTouchInfo iphTouchBuff[PLATFORM_MAX_INPUT];

#endif // _IPHONE_SYSTEM_STUB_H_
