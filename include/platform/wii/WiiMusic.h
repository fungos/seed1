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

/*! \file WiiMusic.h
	\author	Danny Angelo Carminati Grein
	\brief Music Implementation for Wii
*/

#ifndef __WII_MUSIC_H__
#define __WII_MUSIC_H__

#if defined(_WII_)

#include "Defines.h"
#include "File.h"
#include "interface/IMusic.h"
#include "SeedInit.h"

#define MAX_FILENAME	64

namespace Seed { namespace WII {

IResource *MusicResourceLoader(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);

class Music : public IMusic
{
	friend IResource *MusicResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class SoundSystem;

	public:
		Music();
		virtual ~Music();

		// IMusic
		virtual BOOL Update(f32 delta);

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

	protected:
		virtual void Play();
		virtual void Stop();

	private:
		SEED_DISABLE_COPY(Music);

	private:
		File			stMusicFile;
		File			stFileLeftChannel;
		File			stFileRightChannel;

		WiiVoiceData	*pVoiceLeftChannel;
		WiiVoiceData	*pVoiceRightChannel;

		//char			pFileLeftChannel[MAX_FILENAME];
		//char			pFileRightChannel[MAX_FILENAME];
};

}} // namespace

#else // _WII_
	#error "Include 'Music.h' instead 'platform/wii/WiiMusic.h' directly."
#endif // _WII_
#endif // __WII_MUSIC_H__
