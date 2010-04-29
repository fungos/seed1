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

/*! \file IMusic.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Music class interface
*/

#ifndef __IMUSIC_H__
#define __IMUSIC_H__


#include "SeedInit.h"
#include "IResource.h"
#include "MemoryManager.h"
#include "ResourceManager.h"

namespace Seed {

enum eMusicState
{
	MusicNone,
	MusicPlay,
	MusicPlayStarted,
	MusicPlaying,
	MusicStop,
	MusicStopped,
	MusicPause,
	MusicPaused,
	MusicFadeIn,
	MusicFadingIn,
	MusicFadeOut,
	MusicFadingOut,
	MusicFading
};

class SEED_CORE_API IMusic : public IResource
{
	friend class ISoundSystem;
	public:
		IMusic();
		virtual ~IMusic();

		virtual BOOL Update(f32 dt);
		virtual const void *GetData() const;

		virtual void SetVolume(f32 volume);
		virtual f32 GetVolume() const;
		virtual void SetAutoUnload(BOOL b);
		virtual BOOL IsPlaying() const;

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res, IMemoryPool *pool);
		virtual BOOL Unload();

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	protected:
		virtual void Play();
		virtual void Stop();
		virtual void Pause();

		SEED_DISABLE_INSTANCING;

	protected:
		f32					fVolume;
		eMusicState			eState;
		BOOL				bAutoUnload;

	private:
		SEED_DISABLE_COPY(IMusic);
};

} // namespace

#endif // __IMUSIC_H__
