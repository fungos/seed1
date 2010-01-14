/*! \file SdlScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen SDL implementation
*/


#ifndef __SDL_SCREEN_H__
#define __SDL_SCREEN_H__

#include "interface/IScreen.h"

#ifdef _SDL_

#define FADE_OUT_COLOR  0xff
#define FADE_OUT_SOLID  0xff
#define FADE_OUT_TRANS	0x00

#ifdef DEBUG
#define FADE_INCREMENT	0x20
#else
#define FADE_INCREMENT	0x04
#endif // DEBUG

namespace Seed { namespace OGL { class Renderer; class Renderer2D; }}

namespace Seed { namespace SDL {


class IRenderer;


class Screen : public IScreen
{
	friend class OGL::Renderer;
	friend class OGL::Renderer2D;

	public:
		enum eMode
		{
			SCREEN_AUTODETECTW,
			SCREEN_AUTODETECTFS,
			SCREEN_320X240X32W_OPENGL,
			//SCREEN_320X240X32W_SW,
			SCREEN_480x272x32W_OPENGL,
			//SCREEN_480x272x32W_SW,
			SCREEN_480x320x32W_OPENGL,
			SCREEN_IPHONE,
			//SCREEN_480x320x32W_SW,
			SCREEN_640X480X32W_OPENGL,
			SCREEN_WII,
			//SCREEN_640X480X32W_SW,
			SCREEN_800X600X32W_OPENGL,
			//SCREEN_800X600X32W_SW,
			SCREEN_1024X768X32W_OPENGL,
			//SCREEN_1024X768X32W_SW,
			SCREEN_2048X1024X32W_OPENGL,
			//SCREEN_2048X1024X32W_SW,
			SCREEN_320X240X32FS_OPENGL,
			//SCREEN_320X240X32FS_SW,
			SCREEN_480x272x32FS_OPENGL,
			//SCREEN_480x272x32FS_SW,
			SCREEN_480x320x32FS_OPENGL,
			//SCREEN_480x320x32FS_SW,
			SCREEN_640X480X32FS_OPENGL,
			//SCREEN_640X480X32FS_SW,
			SCREEN_800X600X32FS_OPENGL,
			//SCREEN_800X600X32FS_SW,
			SCREEN_1024X768X32FS_OPENGL,
			//SCREEN_1024X768X32FS_SW,
			SCREEN_2048X1024X32FS_OPENGL
			//SCREEN_2048X1024X32FS_SW
		};

		static Screen instance;

	public:
		Screen();
		virtual ~Screen();

		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		// IUpdatable
		virtual BOOL Update(f32 dt);

		virtual void Setup(u32 mode = SCREEN_AUTODETECTFS);
		virtual void SetMode(u32 mode);
		virtual void FadeOut();
		virtual void FadeIn();
		virtual void CancelFade();

		virtual void ToggleFullscreen();
		virtual BOOL HasWindowedMode() const;
		virtual BOOL IsFullscreen() const;

		void SwapSurfaces();
		void ApplyFade();

	protected:
		u32		surfaceSize;
		static	SDL_Surface *pSurface;

	private:
		SEED_DISABLE_COPY(Screen);

		BOOL InitializeVideo();
		void PrepareMode();
		void SetupOpenGL();

#ifdef DEBUG
		void PrintVideoMode();
#endif // DEBUG

	private:
		enum eFadeType
		{
			FADE_IN = 1,
			FADE_OUT
		};

		BOOL		bFullScreen;
		s16 		iFadeStatus;
		eFadeType 	fadeType;
		u8			iBPP;
		u32			iFlags;
		SDL_VideoInfo *videoInfo;
};

Screen *const pScreen = &Screen::instance;


}} // namespace


#else //._SDL_

	#error "Include 'Screen.h' instead 'platform/sdl/SdlScreen.h' directly."

#endif // _SDL_
#endif // __SDL_SCREEN_H__
