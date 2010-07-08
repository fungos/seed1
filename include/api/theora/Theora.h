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

/*! \file Theora.h
	\author	Danny Angelo Carminati Grein
	\brief Theora decoder
*/

#ifndef __THEORA_H__
#define __THEORA_H__

#include "Defines.h"

#if SEED_USE_THEORA == 1

#include "Thread.h"
#include "Mutex.h"
#include "Image.h"
#include "Texture.h"
#include "interface/ISceneObject.h"
#include <oggplay/oggplay.h>

/* FIXME: Somebody please create a Semaphore class! */
#if defined(linux) || defined(SOLARIS) || defined(AIX) || defined(__FreeBSD__) || defined(LINUX)
#if defined(_SDL_)
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_mutex.h>
	#define SEM_CREATE(p,s)		(!(p = (void *)SDL_CreateSemaphore(s)))
	#define SEM_SIGNAL(p)		SDL_SemPost((SDL_sem*)p)
	#define SEM_WAIT(p)			SDL_SemWait((SDL_sem*)p)
	#define SEM_CLOSE(p)		SDL_DestroySemaphore((SDL_sem*)p)
	#define SEM_CHECK(p)		if (p)
	#define SEM_CLEAR(p)		p = 0
	typedef void*				semaphore;
#else
	#warning "Qt Semaphores not tested!"
	#include <QtCore>
	#define SEM_CREATE(p, s)
	#define SEM_SIGNAL(p)		p.release()
	#define SEM_CLOSE(p)
	#define SEM_WAIT(p)			p.acquire()
	#define SEM_CHECK(p)
	#define SEM_CLEAR(p)
	typedef QSemaphore			semaphore;
/*
	#include <semaphore.h>
	#if defined(__FreeBSD__)
		#define SEM_CREATE(p,s) sem_init(&(p), 0, s)
	#else
		#define SEM_CREATE(p,s) sem_init(&(p), 1, s)
	#endif
	#define SEM_SIGNAL(p)	sem_post(&(p))
	#define SEM_WAIT(p)		sem_wait(&(p))
	#define SEM_CLOSE(p)	sem_destroy(&(p))
	typedef sem_t			semaphore;
*/
#endif // _SDL_
#elif defined(WIN32)
#pragma push_macro("Delete")
#pragma push_macro("BOOL")
#pragma push_macro("SIZE_T")
#undef Delete
#undef BOOL
#undef SIZE_T
#include <windows.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("BOOL")
#pragma pop_macro("Delete")
	#define SEM_CREATE(p,s) ((p = CreateSemaphore(NULL, (long)(s), (long)(s), NULL)) == 0)
	#define SEM_SIGNAL(p)   (!ReleaseSemaphore(p, 1, NULL))
	#define SEM_WAIT(p)     WaitForSingleObject(p, INFINITE)
	#define SEM_CLOSE(p)    (!CloseHandle(p))
	#define SEM_CHECK(p)	if (p)
	#define SEM_CLEAR(p)	p = 0
	typedef HANDLE          semaphore;
#elif defined(__APPLE_CC__)
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_mutex.h>
	#define SEM_CREATE(p,s)		(!(p = (void *)SDL_CreateSemaphore(s)))
	#define SEM_SIGNAL(p)		SDL_SemPost((SDL_sem*)p)
	#define SEM_WAIT(p)			SDL_SemWait((SDL_sem*)p)
	#define SEM_CLOSE(p)		SDL_DestroySemaphore((SDL_sem*)p)
	#define SEM_CHECK(p)		if (p)
	#define SEM_CLEAR(p)		p = 0
	typedef void*				semaphore;
/*#elif defined(__APPLE__)
	//#include <Carbon/Carbon.h>
	#define SEM_CREATE(p,s) MPCreateSemaphore(s, s, &(p))
	#define SEM_SIGNAL(p)   MPSignalSemaphore(p)
	#define SEM_WAIT(p)     MPWaitOnSemaphore(p, kDurationForever)
	#define SEM_CLOSE(p)    MPDeleteSemaphore(p)
	#define SEM_CHECK(p)	if (p)
	#define SEM_CLEAR(p)		p = 0
	typedef MPSemaphoreID   semaphore;*/
#endif

namespace Seed {

class SEED_CORE_API Theora : public Thread, public Image /*, public IVideo*/
{
	public:
		Theora();
		virtual ~Theora();

		virtual void Rewind();

		virtual BOOL Load(const char *filename, IMemoryPool *pool);
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual void Reset();
		virtual BOOL Unload();

		virtual BOOL GoToFrame(u32 frame);

		virtual u32 GetFrameCount() const;
		virtual f32 GetFrameRate() const;

		virtual void Play();
		virtual BOOL IsPlaying() const;

		virtual void Stop();
		virtual BOOL IsStopped() const;

		virtual void Pause();
		virtual BOOL IsPaused() const;

		virtual u32 Size() const;

		virtual void PlayToFrame(u32 frame);

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render();

		// Thread
		virtual BOOL Run();

	protected:
		BOOL WaitFrameRate();
		void DoPlay();
		void ProcessVideoData(OggPlayVideoData *data);
		void ConfigureRendering();

	private:
		SEED_DISABLE_COPY(Theora);

	public:
		OggPlay		*pPlayer;
		u8			*pTexData;

		u32			iTime;
		u32			iDuration;
		f32			fFps;
		u32			iFpsDenom;
		u32			iFpsNum;
		f32			fDelay;
		u32			iFrameCount;
		u32			iUntilFrame;
		u64			iLastFrameTime;
		u32			iTrack;
		u32			iTotalFrames;

		u32			iWidth;
		u32			iHeight;

		u32			iUVWidth;
		u32			iUVHeight;

		u32			iTexWidth;
		u32			iTexHeight;

		f32			fTexScaleX;
		f32			fTexScaleY;
		f32			fElapsedTime;

		BOOL		bLoaded;
		BOOL		bPaused;
		BOOL		bPlaying;
		BOOL		bFinished;
		BOOL		bTerminateThread;
		semaphore	sem;

		Texture		cTexture;
		//Image		cImage;
};

} // namespace

#endif // SEED_USE_THEORA

#endif // __THEORA_H__
