/*! \file SdlScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Screen SDL implementation
*/


#ifdef _SDL_

#include "Defines.h"
#include "Screen.h"
#include "System.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Renderer.h"
#include "SeedInit.h"


#define TAG "[Screen] "


namespace Seed { namespace SDL {


Screen Screen::instance;
SDL_Surface *Screen::pSurface = NULL;


Screen::Screen()
	: bFullScreen(FALSE)
	, iFadeStatus(0)
	, fadeType(FADE_IN)
	, iBPP(32)
	, iFlags(0)
	, videoInfo(NULL)
{
	iMode = SCREEN_AUTODETECTFS;
}

Screen::~Screen()
{
	this->Reset();
}

BOOL Screen::Reset()
{
	return TRUE;
}

void Screen::Setup(u32 mode)
{
	this->iMode = mode;
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

	switch (this->iMode)
	{
		case SCREEN_AUTODETECTW:
		{
			//if (videoInfo->hw_available)
				iFlags = SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE;
			//else
			//	iFlags = SDL_DOUBLEBUF | SDL_SWSURFACE;

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

		case SCREEN_AUTODETECTFS:
		{
			//if (videoInfo->hw_available)
				iFlags = SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN;
			//else
			//	iFlags = SDL_DOUBLEBUF | SDL_SWSURFACE | SDL_FULLSCREEN;

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

		case SCREEN_480x272x32W_OPENGL:
		{
			iWidth = 480;
			iHeight = 272;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		}
		break;
/*
		case SCREEN_480x272x32W_SW:
		{
			iWidth = 480;
			iHeight = 272;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF;
		}
		break;
*/
		case SCREEN_IPHONE:
		case SCREEN_480x320x32W_OPENGL:
		{
			iWidth = 480;
			iHeight = 320;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		}
		break;
/*
		case SCREEN_480x320x32W_SW:
		{
			iWidth = 480;
			iHeight = 320;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF;
		}
		break;
*/
		case SCREEN_WII:
		case SCREEN_640X480X32W_OPENGL:
		{
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;// | SDL_FULLSCREEN;
			iWidth = 640;
			iHeight = 480;
		}
		break;
/*
		case SCREEN_640X480X32W_SW:
		{
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF;
			iWidth = 640;
			iHeight = 480;
		}
		break;
*/
		case SCREEN_800X600X32W_OPENGL:
		{
			iWidth = 800;
			iHeight = 600;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		}
		break;
/*
		case SCREEN_800X600X32W_SW:
		{
			iWidth = 800;
			iHeight = 600;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF;
		}
		break;
*/
		case SCREEN_1024X768X32W_OPENGL:
		{
			iWidth = 1024;
			iHeight = 768;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		}
		break;
/*
		case SCREEN_1024X768X32W_SW:
		{
			iWidth = 1024;
			iHeight = 768;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF;
		}
		break;
*/
		case SCREEN_2048X1024X32W_OPENGL:
		{
			iWidth = 2048;
			iHeight = 1024;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		}
		break;
/*
		case SCREEN_2048X1024X32W_SW:
		{
			iWidth = 2048;
			iHeight = 1024;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF;
		}
		break;
*/
		case SCREEN_480x272x32FS_OPENGL:
		{
			iWidth = 480;
			iHeight = 272;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN;
		}
		break;
/*
		case SCREEN_480x272x32FS_SW:
		{
			iWidth = 480;
			iHeight = 272;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
		}
		break;
*/
		case SCREEN_480x320x32FS_OPENGL:
		{
			iWidth = 480;
			iHeight = 320;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN;
		}
		break;
/*
		case SCREEN_480x320x32FS_SW:
		{
			iWidth = 480;
			iHeight = 320;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
		}
		break;
*/
		case SCREEN_640X480X32FS_OPENGL:
		{
			iWidth = 640;
			iHeight = 480;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN;
		}
		break;
/*
		case SCREEN_640X480X32FS_SW:
		{
			iWidth = 640;
			iHeight = 480;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
		}
		break;
*/
		case SCREEN_800X600X32FS_OPENGL:
		{
			iWidth = 800;
			iHeight = 600;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN;
		}
		break;
/*
		case SCREEN_800X600X32FS_SW:
		{
			iWidth = 800;
			iHeight = 600;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
		}
		break;
*/
		case SCREEN_1024X768X32FS_OPENGL:
		{
			iWidth = 1024;
			iHeight = 768;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN;
		}
		break;
/*
		case SCREEN_1024X768X32FS_SW:
		{
			iWidth = 1024;
			iHeight = 768;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
		}
		break;
*/
		case SCREEN_2048X1024X32FS_OPENGL:
		{
			iWidth = 2048;
			iHeight = 1024;
			iFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN;
		}
		break;
/*
		case SCREEN_2048X1024X32FS_SW:
		{
			iWidth = 2048;
			iHeight = 1024;
			iFlags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
		}
		break;
*/
		default:
			Log(TAG "Invalid mode!");
		break;
	}
}

BOOL Screen::Initialize()
{
	Log(TAG "Initializing...");
	this->bFading		= FALSE;
	this->iFadeStatus 	= 16;

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

	bFullScreen = FALSE;
	if (iFlags & SDL_FULLSCREEN)
		bFullScreen = TRUE;

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
	
	if (iFlags & SDL_OPENGL)
	{
		this->SetupOpenGL();
	}

	Uint32 colorkey;
	SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
	if (icon)
	{
		colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
		SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
		SDL_WM_SetIcon(icon, NULL);
	}
	SDL_WM_SetCaption(pSystem->GetApplicationTitle(), pSystem->GetApplicationTitle());//"Seed", "Seed");
	pSurface = SDL_SetVideoMode(iWidth, iHeight, iBPP, iFlags);

	if (!pSurface)
	{
		Log(TAG "Could not set video mode: %s\n", SDL_GetError());
		ret = FALSE;
	}

	int stencil = 0;
	if (SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencil) == -1)
	{
		Log(TAG "Error: %s", SDL_GetError());
	}
	Log(TAG "Stencil buffer: %d", stencil);

	//int val = 0;
	//SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &val);

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

INLINE BOOL Screen::Update(f32 dt)
{
	if (!IScreen::Update(dt))
		return FALSE;

	this->SwapSurfaces();

	return TRUE;
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
	//SDL_UpdateRect(pSurface, 0, 0, iWidth, iHeight);
	//SDL_Flip(pSurface);
	//if (iFlags & SDL_OPENGL)
	SDL_GL_SwapBuffers();
}

void Screen::SetupOpenGL()
{
	// http://sdl.beuc.net/sdl.wiki/SDL_GLattr
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1); // Not in SDL 1.3 - 0 no vsync, 1 vsync
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	/*SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);*/
}

INLINE void Screen::ToggleFullscreen()
{
	// destroy opengl textures
	// change video mode
	// reconfigure opengl context
	// reload textures
#if defined(__linux__)
	SDL_WM_ToggleFullScreen(this->pSurface);
#else
	bFullScreen = !bFullScreen;
	iFlags ^= SDL_FULLSCREEN;
	
	glResourceManager.Unload(Seed::ObjectImage);
	pRenderer->Shutdown();
	this->InitializeVideo();
	//this->Shutdown();
	//this->Initialize();
	pRenderer->Initialize();
	glResourceManager.Reload(Seed::ObjectImage);
#endif
}

INLINE void Screen::SetMode(u32 mode)
{
#if defined(__linux__)
	this->Setup(mode);
	this->PrepareMode();
	this->InitializeVideo();
#else
	glResourceManager.Unload(Seed::ObjectImage);
	pRenderer->Shutdown();
	this->Shutdown();
	IScreen::SetMode(mode);
	this->Initialize();
	pRenderer->Initialize();
	glResourceManager.Reload(Seed::ObjectImage);
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

	glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);

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

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glColor4ub(0, 0, 0, c);

	glPushMatrix();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	//glEnable(GL_TEXTURE_2D);

	glPopAttrib();
}


}} // namespace


#endif // _SDL_
