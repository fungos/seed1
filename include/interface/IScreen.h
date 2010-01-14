/*! \file IScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Screen class interface
*/

#ifndef __ISCREEN_H__
#define __ISCREEN_H__


#include "IModule.h"
#include "IUpdatable.h"


namespace Seed {


class IRenderer;

/// Screen Interface
/**
Any Screen implementation must implement this interface.
*/
class IScreen : public IModule, public IUpdatable
{
	public:
		IScreen();
		virtual ~IScreen();

		/// Setup the screen
		/**
		\param mode Platform dependent video mode.

		<b>Nintendo WII video modes:</b>

		\li \c SCREEN_AUTODETECT Autodetects the supported TV mode.
		\li \c SCREEN_PAL_NORMAL Normal PAL.
		\li \c SCREEN_PAL_INTERLACED Interlaced PAL.
		\li \c SCREEN_MPAL_NORMAL Normal M/PAL.
		\li \c SCREEN_MPAL_INTERLACED Interlaced M/PAL.
		\li \c SCREEN_NTSC_NORMAL Normal NTSC.
		\li \c SCREEN_NTSC_INTERLACED Interlaced NTSC.


		<b>Nintendo DS video modes:</b>

		\li \c SCREEN_MODE_2D_BASIC_FB_NORMAL Main engine on top, Sub engine on bottom (3 text and 1 affine).
		\li \c SCREEN_MODE_2D_BASIC_FB_INVERSE Sub engine on top, Main engine on bottom (3 text and 1 affine).
		\li \c SCREEN_MODE_2D_LARGE_FB_NORMAL Main engine on top, Sub engine on bottom but with a vram allocated for large background (2 text and 2 affine).
		\li \c SCREEN_MODE_2D_LARGE_FB_INVERSE Sub engine on top, Main engine on bottom but with a vram allocated for large background (2 text and 2 affine).


		<b>Apple iPhone 2.1 video modes:</b>
		\li \c NORMAL mode 320x480 up-down.
		\li \c LANDSCAPE Default mode 480x320 landscape with home button to right thumb.
		\li \c LANDSCAPE_GOOFY mode 480x320 landscape with home button to left thumb.
		*/
		virtual void Setup(u32 mode) = 0;

		/// Gets the current video mode
		virtual u32 GetMode() const;

		/// Change the current video mode
		virtual void SetMode(u32 mode);

		/// Start fade out
		virtual void FadeOut() = 0;

		/// Start fade in
		virtual void FadeIn() = 0;

		/// Cancel fade
		virtual void CancelFade() = 0;

		/// Get the screen heigth
		virtual u32 GetHeight() const;

		/// Get the screen width
		virtual u32 GetWidth() const;

		/// Get the aspect ratio
		virtual f32 GetAspectRatio() const;

		/// Verify if a fade is being done.
		virtual BOOL IsFading() const;

		/// Change between fullscreen and windowed for platforms that supports it
		virtual void ToggleFullscreen();

		/// Check if the platform support windowed video mode
		virtual BOOL HasWindowedMode() const;

		/// Check if it is fullscreen
		virtual BOOL IsFullscreen() const;

		/// Sets the actual 2d renderer
		virtual void SetRenderer(IRenderer *renderer); // FIXME: RenderManager

	protected:
		BOOL		bFading;
		u32			iMode;
		IRenderer	*pRenderer;
		f32			fAspectRatio;
		u32			iHeight;
		u32			iWidth;

	private:
		SEED_DISABLE_COPY(IScreen);
};


} // namespace


#endif // __ISCREEN_H__
