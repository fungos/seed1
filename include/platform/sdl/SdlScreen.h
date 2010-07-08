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

/*! \file SdlScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen SDL implementation
*/

#ifndef __SDL_SCREEN_H__
#define __SDL_SCREEN_H__

#if defined(_SDL_)

#include "interface/IScreen.h"
#include "Singleton.h"
#include <SDL/SDL.h>

#define FADE_OUT_COLOR  0xff
#define FADE_OUT_SOLID  0xff
#define FADE_OUT_TRANS	0x00

#if defined(DEBUG)
#define FADE_INCREMENT	0x20
#else
#define FADE_INCREMENT	0x04
#endif // DEBUG

namespace Seed { namespace SDL {

class SEED_CORE_API Screen : public IScreen
{
	SEED_SINGLETON_DECLARE(Screen);
	public:
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

		void SetMode(eVideoMode mode);
		virtual void FadeOut();
		virtual void FadeIn();
		virtual void CancelFade();

		virtual void ToggleFullscreen();
		virtual bool HasWindowedMode() const;
		virtual bool IsFullscreen() const;

		void SwapSurfaces();
		void ApplyFade();

		// IScreen
		virtual void Update();

		// HACK - test
		int			iHandle;

	protected:
		int		surfaceSize;
		SDL_Surface *pSurface;

	private:
		SEED_DISABLE_COPY(Screen);

		BOOL InitializeVideo();
		void PrepareMode();
		void SetupOpenGL();

#if defined(DEBUG)
		void PrintVideoMode();
#endif // DEBUG

	private:
		enum eFadeType
		{
			FADE_IN = 1,
			FADE_OUT
		};

		bool		bFullScreen;
		int 		iFadeStatus;
		eFadeType 	fadeType;
		int			iBPP;
		int			iFlags;
		SDL_VideoInfo *videoInfo;
};

#define pScreen Screen::GetInstance()

}} // namespace

#else //._SDL_
	#error "Include 'Screen.h' instead 'platform/sdl/SdlScreen.h' directly."
#endif // _SDL_
#endif // __SDL_SCREEN_H__
