/*! \file IphScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen Iphone Implementation
*/

#ifndef __IPH_SCREEN_H__
#define __IPH_SCREEN_H__

#ifdef _IPHONE_

#include "interface/IScreen.h"
#include "interface/IRenderer2D.h"
#include <OpenGLES/ES1/gl.h>

namespace Seed { namespace iPhone {

class IRenderer2D;

class Screen : public IScreen
{
	public:
		Screen();
		virtual ~Screen();

		// IScreen
		virtual void Setup(u32 mode = LANDSCAPE);
		virtual void FadeOut();
		virtual void FadeIn();
		virtual u32 GetHeight() const;
		virtual u32 GetWidth() const;
		virtual void SetRenderer2D(IRenderer2D *renderer);
		virtual void CancelFade();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		// IUpdatable
		virtual BOOL Update(f32 delta);

	public:
		enum eScreenMode
		{
			NORMAL,
			LANDSCAPE,
			LANDSCAPE_GOOFY
		};

	public:
		static Screen instance;

	protected:
		friend class Renderer;
		friend class Renderer2D;

		/* OpenGL names for the renderbuffer and framebuffers used to render to this view */
		GLuint		renderBuffer;
		GLuint		frameBuffer;
		GLuint		depthRenderbuffer;

	private:
		SEED_DISABLE_COPY(Screen);

		void CreateHardwareSurfaces();
		void DestroyHardwareSurfaces();
		void SwapSurfaces();
		void ApplyFade();

	private:
		enum eFadeType
		{
			FADE_IN = 1,
			FADE_OUT
		};

	private:
		s32 		iFadeStatus;
		eFadeType 	fadeType;
		GLint		iHeight;
		GLint		iWidth;
		GLint		iModeHeight;
		GLint		iModeWidth;

		IRenderer2D *pRenderer;
};

Screen *const pScreen = &Screen::instance;

}} // namespace

#else // _IPHONE_
	#error "Include 'Screen.h' instead 'platform/iphone/IphScreen.h' directly."
#endif // _IPHONE_
#endif // __IPH_SCREEN_H__
