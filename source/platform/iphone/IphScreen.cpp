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
#include "IphoneView.h"

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
	, pRenderer(NULL)
{
	this->iMode = LANDSCAPE;
	this->Reset();
}

Screen::~Screen()
{
	this->Reset();
}

BOOL Screen::Reset()
{
	this->bFading    	= FALSE;
	this->iFadeStatus 	= FADE_OUT_TRANS;

	return TRUE;
}

void Screen::Setup(u32 mode)
{
	this->iMode = mode;
}

BOOL Screen::Initialize()
{
	Log(TAG "Initializing...");
	this->CreateHardwareSurfaces();
	this->Reset();
	Info(TAG "Video resolution is %dx%d.", this->iModeWidth, this->iModeHeight);
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

BOOL Screen::Update(f32 delta)
{
	UNUSED(delta);
	this->SwapSurfaces();
	return TRUE;
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

#if SEED_ENABLE_DEPTH_TEST
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, iWidth, iHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
#endif // SEED_ENABLE_DEPTH_TEST

	if (iMode == LANDSCAPE || iMode == LANDSCAPE_GOOFY)
	{
		this->iModeWidth = this->iHeight;
		this->iModeHeight = this->iWidth;
	}
	else
	{
		this->iModeWidth = this->iHeight;
		this->iModeHeight = this->iWidth;
	}
}

void Screen::DestroyHardwareSurfaces()
{
	glDeleteFramebuffersOES(1, &frameBuffer);
	frameBuffer = 0;
	glDeleteRenderbuffersOES(1, &renderBuffer);
	renderBuffer = 0;

#if SEED_ENABLE_DEPTH_TEST
	if (depthRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
#endif // SEED_ENABLE_DEPTH_TEST
}

void Screen::ApplyFade()
{
	if (this->bFading == FALSE)
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

	const GLfloat vertices[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
/*
	const GLubyte colors[] =
	{
		FADE_OUT_COLOR, FADE_OUT_COLOR, FADE_OUT_COLOR, c,
		FADE_OUT_COLOR, FADE_OUT_COLOR, FADE_OUT_COLOR, c,
		FADE_OUT_COLOR, FADE_OUT_COLOR, FADE_OUT_COLOR, c,
		FADE_OUT_COLOR, FADE_OUT_COLOR, FADE_OUT_COLOR, c,
	};

	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
*/
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4ub(255, 255, 255, c);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

INLINE u32 Screen::GetHeight() const
{
	return static_cast<u32>(iModeHeight);
}

INLINE u32 Screen::GetWidth() const
{
	return static_cast<u32>(iModeWidth);
}

}} // namespace

#endif // _IPHONE_
