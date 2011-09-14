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

/*! \file QtScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Screen QT implementation
*/

#if defined(_QT_)

#include "Defines.h"
#include "Screen.h"
#include "System.h"
#include "Log.h"
#include "MemoryManager.h"

/*
##if !defined(GL_MULTISAMPLE)
#define GL_MULTISAMPLE  0x809D
#endif // GL_MULTISAMPLE
*/

#define TAG "[Screen] "

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(Screen)

Scene *Screen::pScene = NULL;

Screen::Screen()
{
	this->Reset();
}

Screen::~Screen()
{
	this->Reset();
}

BOOL Screen::Reset()
{
	IModule::Reset();
	iFadeStatus = 0;

	return TRUE;
}

void Screen::PrepareMode()
{
	iWidth = 800;
	iHeight = 600;
	iFlags = 0;
	iBPP = 32;

	switch (nMode)
	{
		case Video_480x272:
		{
			iWidth = 480;
			iHeight = 272;
		}
		break;

		case Video_iPhonePortrait:
		{
			iWidth = 320;
			iHeight = 480;
		}
		break;

		case Video_iPhoneLandscape:
		case Video_iPhoneLandscapeGoofy:
		case Video_480x320:
		{
			iWidth = 480;
			iHeight = 320;
		}
		break;

		case Video_NintendoWii:
		case Video_640x480:
		{
			iWidth = 640;
			iHeight = 480;
		}
		break;

		case Video_AutoDetect:
		case Video_800x600:
		{
			iWidth = 800;
			iHeight = 600;
		}
		break;

		case Video_1024x768:
		{
			iWidth = 1024;
			iHeight = 768;
		}
		break;

		default:
			Info(TAG "Invalid video mode, using 800x600.");
		break;
	}
}

BOOL Screen::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	this->PrepareMode();

	if (!pScene)
		pScene = New(Scene(iWidth, iHeight));

	this->Reset();
	this->CreateHardwareSurfaces();

	Log(TAG "Video resolution is %dx%dx%d.", iWidth, iHeight, iBPP);
	Log(TAG "Initialization completed.");

	bFullScreen = FALSE;
	bFading = FALSE;

	return TRUE;
}

BOOL Screen::Shutdown()
{
	Log(TAG "Terminating...");
	BOOL r = this->Reset();

	Delete(pScene);
	pScene = NULL;
	IModule::Shutdown();
	Log(TAG "Terminated.");

	return r;
}

INLINE void Screen::Update()
{
	//IScreen::Update(); // abstract
	this->pScene->update();
}

INLINE void Screen::FadeOut()
{
	if (bFading)
		return;

	bFading		= TRUE;
	fadeType	= FADE_OUT;
	iFadeStatus	= FADE_OUT_TRANS;
}

INLINE void Screen::FadeIn()
{
	if (bFading)
		return;

	bFading		= TRUE;
	fadeType	= FADE_IN;
	iFadeStatus	= FADE_OUT_SOLID;
}

INLINE void Screen::CancelFade()
{
	bFading		= FALSE;
	iFadeStatus	= FADE_OUT_TRANS;
}

INLINE void Screen::SwapSurfaces()
{
}

INLINE void Screen::CreateHardwareSurfaces()
{
}

INLINE void Screen::DestroyHardwareSurfaces()
{
}

INLINE u32 Screen::GetHeight() const
{
	return this->iHeight;
}

INLINE u32 Screen::GetWidth() const
{
	return this->iWidth;
}

INLINE void Screen::ToggleFullscreen()
{
	// destroy opengl textures
	// change video mode
	// reconfigure opengl context
	// reload textures
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

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
		glVertex3i(-1, -1, -1);
		glVertex3i(1, -1, -1);
		glVertex3i(1, 1, -1);
		glVertex3i(-1, 1, -1);
	glEnd();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	u8 c = static_cast<u8>(iFadeStatus & 0xff);

	const GLfloat vertices[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4ub(255, 255, 0, c);
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

INLINE void *Screen::GetSurface() const
{
	return this->pScene;
}

}} // namespace

#endif // _QT_
