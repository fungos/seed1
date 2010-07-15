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

/*! \file IphScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Screen Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Screen.h"
#include "Log.h"
#include "MemoryManager.h"
#include "SeedInit.h"
#include "RendererDevice.h"

#undef WideChar
#include "platform/iphone/IphoneView.h"

#define FADE_OUT_COLOR  0xff
#define FADE_OUT_SOLID  0xff
#define FADE_OUT_TRANS	0x00

#if defined(DEBUG)
#define FADE_INCREMENT	0x10
#else
#define FADE_INCREMENT	0x10
#endif // DEBUG

#define TAG "[Screen] "

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(Screen);

Screen::Screen()
	: renderBuffer()
	, frameBuffer()
	, depthRenderbuffer()
	, iFadeStatus(0)
	, fadeType(FADE_IN)
	, iHeight(0)
	, iWidth(0)
	, iModeHeight(0)
	, iModeWidth(0)
{
	this->Reset();
}

Screen::~Screen()
{
	this->Reset();
}

BOOL Screen::Reset()
{
	bFading    	= FALSE;
	iFadeStatus 	= FADE_OUT_TRANS;

	return TRUE;
}

BOOL Screen::Initialize()
{
	Log(TAG "Initializing...");
	
	nMode = pConfiguration->GetVideoMode();

	this->CreateHardwareSurfaces();
	this->Reset();
	
	Info(TAG "Video resolution is %dx%d.", iModeWidth, iModeHeight);
	Log(TAG "Initialization completed.");

	return TRUE;
}

BOOL Screen::Shutdown()
{
	Log(TAG "Terminating...");
	
	BOOL r = this->Reset();
	this->DestroyHardwareSurfaces();
	
	Log(TAG "Terminated.");

	return r;
}

void Screen::Update()
{
	this->SwapSurfaces();
}

void Screen::FadeOut()
{
	if (bFading)
		return;

	bFading		= TRUE;
	fadeType	= FADE_OUT;
	iFadeStatus	= FADE_OUT_TRANS;
}

void Screen::FadeIn()
{
	if (bFading)
		return;

	bFading		= TRUE;
	fadeType	= FADE_IN;
	iFadeStatus	= FADE_OUT_SOLID;
}

void Screen::CancelFade()
{
	if (!bFading)
		return;

	bFading		= FALSE;
	iFadeStatus	= FADE_OUT_TRANS;
}

void Screen::SwapSurfaces()
{
	glFlush();
}

void Screen::CreateHardwareSurfaces()
{
	glGenFramebuffersOES(1, &frameBuffer);
	glGenRenderbuffersOES(1, &renderBuffer);

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderBuffer);
	iphPrepareGLContext();
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderBuffer);

	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &iWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &iHeight);
	
	if (nMode == Seed::Video_iPhoneLandscape || nMode == Seed::Video_iPhoneLandscapeGoofy)
	{
		iModeWidth = iHeight;
		iModeHeight = iWidth;
	}
	else
	{
		iModeWidth = iWidth;
		iModeHeight = iHeight;
	}
	
	iHeight = iModeHeight;
	iWidth = iModeWidth;
	fAspectRatio = (f32)iHeight / (f32)iWidth;
}

void Screen::DestroyHardwareSurfaces()
{
	glDeleteFramebuffersOES(1, &frameBuffer);
	frameBuffer = 0;
	glDeleteRenderbuffersOES(1, &renderBuffer);
	renderBuffer = 0;
}

void Screen::ApplyFade()
{
	if (bFading == FALSE)
		return;

	if (fadeType == FADE_IN)
	{
		iFadeStatus -= FADE_INCREMENT;

		if (iFadeStatus <= FADE_OUT_TRANS)
		{
			bFading = FALSE;
			iFadeStatus = FADE_OUT_TRANS;
		}
	}
	else
	{
		iFadeStatus += FADE_INCREMENT;

		if (iFadeStatus >= FADE_OUT_SOLID)
		{
			bFading = FALSE;
			iFadeStatus = FADE_OUT_SOLID;
		}
	}

	u8 c = static_cast<u8>(iFadeStatus & 0xff);
	pRendererDevice->BackbufferFill(PIXEL_COLOR(0u, 0u, 0u, c));
}

}} // namespace

#endif // _IPHONE_
