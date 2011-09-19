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

/*! \file PS3Screen.cpp
	\author	Danny Angelo Carminati Grein
	\brief ps3dev screen implementation
*/

#if defined(_PS3DEV_)

#include "Screen.h"
#include "Log.h"
#include "SeedInit.h"
#include "RendererDevice.h"

#define TAG "[Screen] "

namespace Seed { namespace PS3 {

SEED_SINGLETON_DEFINE(Screen);

Screen::Screen()
	: iHandle(0)
	, surfaceSize(0)
	, bFullScreen(FALSE)
	, iFadeStatus(0)
	, fadeType(FADE_IN)
	, iBPP(32)
	, iFlags(0)
	, videoInfo(NULL)
{
}

Screen::~Screen()
{
	this->Reset();
}

BOOL Screen::Reset()
{
	return TRUE;
}

void Screen::PrepareMode()
{
	videoInfo = const_cast<SDL_VideoInfo *>(SDL_GetVideoInfo());
	if (videoInfo)
	{
		Info(TAG "SDL Video Info:");
		Info(TAG "\tHardware surface available...: %s", videoInfo->hw_available ? "yes" : "no");
		Info(TAG "\tWindow manager available.....: %s", videoInfo->wm_available ? "yes" : "no");
		Info(TAG "\tHardware blit accelerated....: %s", videoInfo->blit_hw ? "yes" : "no");
		Info(TAG "\tHardware colorkey blit.......: %s", videoInfo->blit_hw_CC ? "yes" : "no");
		Info(TAG "\tHardware alpha blit..........: %s", videoInfo->blit_hw_A ? "yes" : "no");
		Info(TAG "\tSoftware to hardware blit....: %s", videoInfo->blit_sw ? "yes" : "no");
		Info(TAG "\tSoftware to hardware colorkey: %s", videoInfo->blit_sw_CC ? "yes" : "no");
		Info(TAG "\tSoftware to hardware alpha...: %s", videoInfo->blit_sw_A ? "yes" : "no");
		Info(TAG "\tColor fill accelerated.......: %s", videoInfo->blit_fill ? "yes" : "no");
		Info(TAG "\tDisplay pixel format.........: ");
		Info(TAG "\t\tBytes per pixel............: %d", videoInfo->vfmt->BytesPerPixel);
		Info(TAG "\t\tRGBA loss..................: %d %d %d %d", videoInfo->vfmt->Rloss, videoInfo->vfmt->Gloss, videoInfo->vfmt->Bloss, videoInfo->vfmt->Aloss);
		Info(TAG "\tBest resolution..............: %dx%d", videoInfo->current_w, videoInfo->current_h);
		Info(TAG "\tTotal video memory available.: %d", videoInfo->video_mem);
	}

	switch (nMode)
	{
		case Video_AutoDetect:
		{
			if (videoInfo)
			{
				iWidth = videoInfo->current_w;
				iHeight = videoInfo->current_h;
				iBPP = videoInfo->vfmt->BitsPerPixel;
			}
			else
			{
				Log(TAG "Error: Failed to auto detect video mode.");
				return;
			}
		}
		break;

		case Video_320x240:
		{
			iWidth = 320;
			iHeight = 240;
		}
		break;

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

		case Video_800x600:
		{
			iWidth = 800;
			iHeight = 600;
		}
		break;

		case Video_1024x600:
		{
			iWidth = 1024;
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
			Log(TAG "Invalid video mode!");
		break;
	}

	fAspectRatio = (f32)iHeight / (f32)iWidth;
	iFlags = SDL_DOUBLEBUF | SDL_HWSURFACE;
}

bool Screen::Initialize()
{
	Log(TAG "Initializing...");

	IScreen::SetMode(pConfiguration->GetVideoMode());
	bFading = FALSE;
	iFadeStatus = 16;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		Log(TAG "ERROR: Failed to initialize screen.");
		Log(TAG "Initialization failed.");
		return FALSE;
	}

	this->PrepareMode();
	if (!videoInfo)
	{
		Log(TAG "ERROR: You must set up a video mode!");
		return FALSE;
	}

	if (!this->InitializeVideo())
		return FALSE;

	Info(TAG "Video resolution is %dx%dx%d.", iWidth, iHeight, iBPP);

#if defined(DEBUG)
	SDL_ShowCursor(1);
#else
	SDL_ShowCursor(0);
#endif // DEBUG

	Log(TAG "Initialization completed.");
	return TRUE;
}

INLINE BOOL Screen::InitializeVideo()
{
	BOOL ret = TRUE;

	if (pSurface)
	{
		SDL_FreeSurface(pSurface);
		pSurface = NULL;
	}

	// Setup RSX?

	SDL_WM_SetCaption(pConfiguration->GetApplicationTitle(), pConfiguration->GetApplicationTitle());
	pSurface = SDL_SetVideoMode(iWidth, iHeight, iBPP, iFlags);
	if (!pSurface)
	{
		Log(TAG "Could not set video mode: %s\n", SDL_GetError());
		ret = FALSE;
	}
	else
	{
		SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
		if (icon)
		{
			Uint32 colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
			SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
			SDL_WM_SetIcon(icon, NULL);
		}
	}

	return ret;
}

BOOL Screen::Shutdown()
{
	Log(TAG "Terminating...");

	iFadeStatus = 0;
	fadeType	= FADE_IN;
	iHeight		= PLATFORM_MAX_SCREEN_HEIGHT;
	iWidth		= PLATFORM_MAX_SCREEN_WIDTH;
	iBPP		= 32;
	iFlags		= 0;
	videoInfo	= NULL;

	if (pSurface)
		SDL_FreeSurface(pSurface);
	pSurface = NULL;

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	Log(TAG "Terminated.");

	return TRUE;
}

INLINE void Screen::Update()
{
	this->SwapSurfaces();
	pRendererDevice->Update();
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
	if (iFlags & SDL_OPENGL)
		SDL_GL_SwapBuffers();
}

INLINE void Screen::ToggleFullscreen()
{
}

INLINE void Screen::SetMode(eVideoMode mode)
{
	IScreen::SetMode(mode);
	this->PrepareMode();
	this->InitializeVideo();
}

INLINE BOOL Screen::HasWindowedMode() const
{
	return TRUE;
}

INLINE BOOL Screen::IsFullscreen() const
{
	return bFullScreen;
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

#endif // _PS3DEV_
