/*! \file WiiScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen initialization
*/

#ifndef __WII_SCREEN_H__
#define __WII_SCREEN_H__


#include "Defines.h"


#ifdef _WII_


#include "interface/IScreen.h"
#include "interface/IRenderer.h"


namespace Seed { namespace WII {


class Renderer2D;
class Renderer3D;


class Screen : public IScreen
{
	friend class Renderer;
	friend class Renderer2D;
	friend class Renderer3D;
	friend class HomeButton;

	protected:
		u32			surfaceSize;
		static void *pSurfaceA;
		static void *pSurfaceB;
		static void *currentSurface;
		WiiRenderModeObj renderMode;
		IRenderer 	*pRenderer;

	private:
		enum
		{
			SCREEN_AUTODETECT,
			SCREEN_PAL_NORMAL,
			SCREEN_PAL_INTERLACED,
			SCREEN_MPAL_NORMAL,
			SCREEN_MPAL_INTERLACED,
			SCREEN_NTSC_NORMAL,
			SCREEN_NTSC_INTERLACED
		};

		enum eFadeType
		{
			FADE_IN = 1,
			FADE_OUT
		};

		PIXEL 		iFadeStatus;
		eFadeType 	fadeType;
		BOOL 		bFirstFrame;

		void CreateHardwareSurfaces();
		void DestroyHardwareSurfaces();
		void ApplyFade();

#ifdef DEBUG
		void PrintVideoMode();
#endif // DEBUG
	public:
		Screen();
		virtual ~Screen();

		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		// IUpdatable
		virtual BOOL Update();

		virtual void Setup(u32 mode = SCREEN_AUTODETECT);
		virtual void FadeOut();
		virtual void FadeIn();
		virtual void CancelFade();
		void SwapSurfaces();

		virtual u32 GetHeight() const;
		virtual u32 GetWidth() const;
		virtual void SetRenderer(IRenderer *renderer);

		static Screen instance;
};

Screen *const pScreen = &Screen::instance;


}} // namespace


#else // _WII_

	#error "Include 'Screen.h' instead 'platform/wii/WiiScreen.h' directly."

#endif // _WII_
#endif // __WII_SCREEN_H__
