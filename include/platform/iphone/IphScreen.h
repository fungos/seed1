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

/*! \file IphScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen Iphone Implementation
*/

#ifndef __IPH_SCREEN_H__
#define __IPH_SCREEN_H__

#if defined(_IPHONE_)

#include "interface/IScreen.h"
#include "Singleton.h"

#include <OpenGLES/ES1/gl.h>

namespace Seed { namespace iPhone {

class Screen : public IScreen
{
	SEED_SINGLETON_DECLARE(Screen)
	public:
		void ApplyFade();
		virtual void Update();

		// IScreen
		virtual void FadeOut();
		virtual void FadeIn();
		virtual void CancelFade();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	public:
		/* OpenGL names for the renderbuffer and framebuffers used to render to this view */
		GLuint		renderBuffer;
		GLuint		frameBuffer;
		GLuint		depthRenderbuffer;

	private:
		SEED_DISABLE_COPY(Screen);

		void CreateHardwareSurfaces();
		void DestroyHardwareSurfaces();
		void SwapSurfaces();

	private:
		enum eFadeType
		{
			FADE_IN = 1,
			FADE_OUT
		};

		s32 		iFadeStatus;
		eFadeType 	fadeType;
		GLint		iHeight;
		GLint		iWidth;
		GLint		iModeHeight;
		GLint		iModeWidth;
};

#define pScreen Screen::GetInstance()

}} // namespace

#else // _IPHONE_

	#error "Include 'Screen.h' instead 'platform/iphone/IphScreen.h' directly."

#endif // _IPHONE_
#endif // __IPH_SCREEN_H__
