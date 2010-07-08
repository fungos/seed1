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

/*! \file QtScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen QT implementation
*/

#ifndef __QT_SCREEN_H__
#define __QT_SCREEN_H__

#include "interface/IScreen.h"

#if defined(_QT_)

#define FADE_OUT_COLOR  0xff
#define FADE_OUT_SOLID  0xff
#define FADE_OUT_TRANS	0x00

#if defined(DEBUG)
#define FADE_INCREMENT	0x04
#else
#define FADE_INCREMENT	0x20
#endif // DEBUG

#include <QPainter>
#include <QGraphicsScene>
#include "QtScene.h"

#include "Renderer.h"
#include "Singleton.h"

namespace Seed { namespace QT {

class IRenderer;

class Screen : public IScreen
{
	friend class Renderer;
	friend class Renderer2D;

	SEED_SINGLETON_DECLARE(Screen);

	public:
		enum eMode
		{
			SCREEN_AUTODETECTW,
			SCREEN_AUTODETECTFS,
			SCREEN_320X240X32W_OPENGL,
			SCREEN_PSP,
			SCREEN_480x272x32W_OPENGL,
			SCREEN_480x320x32W_OPENGL,
			SCREEN_IPHONE,
			SCREEN_640X480X32W_OPENGL,
			SCREEN_WII,
			SCREEN_800X600X32W_OPENGL,
			SCREEN_1024X768X32W_OPENGL,
			SCREEN_2048X1024X32W_OPENGL,
			SCREEN_320X240X32FS_OPENGL,
			SCREEN_480x272x32FS_OPENGL,
			SCREEN_480x320x32FS_OPENGL,
			SCREEN_640X480X32FS_OPENGL,
			SCREEN_800X600X32FS_OPENGL,
			SCREEN_1024X768X32FS_OPENGL,
			SCREEN_2048X1024X32FS_OPENGL
		};

	public:
		virtual void Setup(u32 mode = SCREEN_AUTODETECTFS);
		virtual void FadeOut();
		virtual void FadeIn();
		virtual void CancelFade();

		virtual u32 GetHeight() const;
		virtual u32 GetWidth() const;
		virtual void ToggleFullscreen();
		void SwapSurfaces();
		void *GetSurface() const;
		void ApplyFade();

		// IScreen
		virtual void Update();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	protected:
		u32		surfaceSize;
		static Scene *pScene;

	private:
		SEED_DISABLE_COPY(Screen);

		void PrepareMode();
		void CreateHardwareSurfaces();
		void DestroyHardwareSurfaces();

#if defined(DEBUG)
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
		u32			iHeight;
		u32			iWidth;
		u8			iBPP;
		u32			iFlags;
		//Renderer2D *pRenderer2D;
		//Renderer 	*pRenderer;
		//QColor		cBackgroundColor;
};

extern "C" {
SEED_CORE_API SEED_SINGLETON_EXTERNALIZE(Screen);
}

}} // namespace

#else //._QT_
	#error "Include 'Screen.h' instead 'platform/qt/QtScreen.h' directly."
#endif // _QT_
#endif // __QT_SCREEN_H__
