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

/*! \file WiiScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen initialization
*/

#ifndef __WII_SCREEN_H__
#define __WII_SCREEN_H__

#include "Defines.h"

#if defined(_WII_)

#include "interface/IScreen.h"
#include "interface/IRenderer.h"
#include "Singleton.h"

namespace Seed { namespace WII {

class Renderer2D;
class Renderer3D;

class Screen : public IScreen
{
	friend class Renderer;
	friend class Renderer2D;
	friend class Renderer3D;
	friend class HomeButton;

	SEED_SINGLETON_DECLARE(Screen);
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

	protected:
		u32			surfaceSize;
		static void *pSurfaceA;
		static void *pSurfaceB;
		static void *currentSurface;
		WiiRenderModeObj renderMode;
		IRenderer 	*pRenderer;

	private:
		SEED_DISABLE_COPY(Screen);

		void CreateHardwareSurfaces();
		void DestroyHardwareSurfaces();
		void ApplyFade();

#if defined(DEBUG)
		void PrintVideoMode();
#endif // DEBUG

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
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Screen);
}

}} // namespace

#else // _WII_
	#error "Include 'Screen.h' instead 'platform/wii/WiiScreen.h' directly."
#endif // _WII_
#endif // __WII_SCREEN_H__
