/*! \file QtScreen.h
	\author	Danny Angelo Carminati Grein
	\brief Screen QT implementation
*/


#ifndef __QT_SCREEN_H__
#define __QT_SCREEN_H__

#include "interface/IScreen.h"

#ifdef _QT_

#define FADE_OUT_COLOR  0xff
#define FADE_OUT_SOLID  0xff
#define FADE_OUT_TRANS	0x00

#ifdef DEBUG
#define FADE_INCREMENT	0x04
#else
#define FADE_INCREMENT	0x20
#endif // DEBUG


#include <QPainter>
#include <QGraphicsScene>
#include "QtScene.h"

#include "Renderer.h"


namespace Seed { namespace QT {


class IRenderer;


class Screen : public IScreen
{
	friend class Renderer;
	friend class Renderer2D;

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

		static Screen instance;

	public:
		Screen();
		virtual ~Screen();

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

		// IUpdatable
		virtual BOOL Update(f32 dt);

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
		u32			iHeight;
		u32			iWidth;
		u8			iBPP;
		u32			iFlags;
		//Renderer2D *pRenderer2D;
		//Renderer 	*pRenderer;
		//QColor		cBackgroundColor;
};


Screen *const pScreen = &Screen::instance;


}} // namespace


#else //._QT_

	#error "Include 'Screen.h' instead 'platform/qt/QtScreen.h' directly."

#endif // _QT_
#endif // __QT_SCREEN_H__
