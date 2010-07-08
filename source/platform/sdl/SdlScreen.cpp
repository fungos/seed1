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

/*! \file SdlScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Screen SDL implementation
*/

#if defined(_SDL_)

#include "Screen.h"
#include "Log.h"
#include "SeedInit.h"
#include "RendererDevice.h"

#if defined(WIN32)
#pragma push_macro("Delete")
#pragma push_macro("BOOL")
#pragma push_macro("SIZE_T")
#undef Delete
#undef BOOL
#undef SIZE_T
#include <SDL/SDL_syswm.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("BOOL")
#pragma pop_macro("Delete")
#endif

#define TAG "[Screen] "

namespace Seed { namespace SDL {

SEED_SINGLETON_DEFINE(Screen);

Screen::Screen()
	: surfaceSize(0)
	, bFullScreen(FALSE)
	, iFadeStatus(0)
	, fadeType(FADE_IN)
	, iBPP(32)
	, iFlags(0)
	, videoInfo(NULL)
	, iHandle(0)
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
		Info(TAG "\t\tColorkey...................: %x", videoInfo->vfmt->colorkey);
		Info(TAG "\t\talpha......................: %d", videoInfo->vfmt->alpha);
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

		case Video_iPhone:
		case Video_iPhoneLandscape:
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

	eRendererDeviceType type = pConfiguration->GetRendererDeviceType();
	if (type == Seed::RendererDeviceOpenGL14 || type == Seed::RendererDeviceOpenGL20 ||
		type == Seed::RendererDeviceOpenGL30 || type == Seed::RendererDeviceOpenGL40)
	{
		this->SetupOpenGL();
	}

#if defined(WIN32)
	int dpiX = 0, dpiY = 0;
	HDC hdc = GetDC(NULL);
	if (hdc)
	{
		dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	int scaleX = MulDiv(cxScreen, USER_DEFAULT_SCREEN_DPI, dpiX);
	int scaleY = MulDiv(cyScreen, USER_DEFAULT_SCREEN_DPI, dpiY);

	Info(TAG "Desktop DPI: %dx%d", dpiX, dpiY);
	Info(TAG "Desktop Resolution: %dx%d", cxScreen, cyScreen);
	Info(TAG "Desktop DPI scaled resolution: %dx%d", scaleX, scaleY);

	if (cxScreen <= 1024 && cyScreen <= 768 && iWidth > 800 && iHeight > 600)
	{
		iWidth = 800;
		iHeight = 600;
	}
#endif

	SDL_WM_SetCaption(pConfiguration->GetApplicationTitle(), pConfiguration->GetApplicationTitle());
	pSurface = SDL_SetVideoMode(iWidth, iHeight, iBPP, iFlags);
	if (!pSurface)
	{
		Log(TAG "Could not set video mode: %s\n", SDL_GetError());
		ret = FALSE;
	}
	else
	{
#if defined(WIN32)
		BOOL icon = FALSE;

		/*
		If there is a icon.ico file in the SAME directory of the executable (can't be workdir) we will use it, otherwise we will look for
		<workdir>/data/icon.bmp and use it instead.
		*/
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if (SDL_GetWMInfo(&info) > 0)
		{
			HWND hWnd = info.window;
			iHandle = (u32)hWnd;

			const HANDLE bigIcon = ::LoadImage(NULL, "icon.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
			if (bigIcon)
			{
				icon = TRUE;
				::SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)bigIcon);
			}

			const HANDLE lilIcon = ::LoadImage(NULL, "icon.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
			if (bigIcon)
			{
				icon = TRUE;
				::SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)lilIcon);
			}
		}
		if (!icon)
#endif
		{
			SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
			if (icon)
			{
				Uint32 colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
				SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
				SDL_WM_SetIcon(icon, NULL);
			}
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

void Screen::SetupOpenGL()
{
	iFlags |= SDL_OPENGL;

	// http://sdl.beuc.net/sdl.wiki/SDL_GLattr
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}

INLINE void Screen::ToggleFullscreen()
{
	// destroy opengl textures
	// change video mode
	// reconfigure opengl context
	// reload textures
#if defined(__linux__)
	SDL_WM_ToggleFullScreen(pSurface);
#else
	bFullScreen = !bFullScreen;
	iFlags ^= SDL_FULLSCREEN;
	
	pResourceManager->Unload(Seed::ObjectTexture);
	pRendererDevice->Shutdown();
	this->InitializeVideo();
	pRendererDevice->Initialize();
	pResourceManager->Reload(Seed::ObjectTexture);

#if defined(WIN32)
	if (!bFullScreen)
	{
		RECT rcTmp,rectWindow;
		GetClientRect(GetActiveWindow(), &rcTmp);
		rectWindow.left = 0;
		rectWindow.top = 0;
		rectWindow.right = rcTmp.right;
		rectWindow.bottom = rcTmp.bottom;

		SetWindowPos(GetActiveWindow(), HWND_TOP, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom, SWP_SHOWWINDOW);
	}
#endif
#endif
}

INLINE void Screen::SetMode(eVideoMode mode)
{
#if defined(__linux__)
	IScreen::SetMode(mode);
	this->PrepareMode();
	this->InitializeVideo();
#else
	pResourceManager->Unload(Seed::ObjectTexture);
	pRendererDevice->Shutdown();
	this->Shutdown();
	IScreen::SetMode(mode);
	this->Initialize();
	pRendererDevice->Initialize();
	pResourceManager->Reload(Seed::ObjectTexture);
#endif
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

#endif // _SDL_
