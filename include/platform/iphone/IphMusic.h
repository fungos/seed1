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

/*! \file IphMusic.h
	\author	Danny Angelo Carminati Grein
	\brief Music Implementation for iPhone
*/

#ifndef __IPHONE_MUSIC_H__
#define __IPHONE_MUSIC_H__

#include "Defines.h"

#if defined(_IPHONE_)

#include "File.h"
#include "interface/IMusic.h"
#include "SeedInit.h"

namespace Seed { namespace iPhone {

IResource *MusicResourceLoader(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);

class SEED_CORE_API Music : public IMusic
{
	friend IResource *MusicResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);
	friend class SoundSystem;

	public:
		Music();
		virtual ~Music();

		// IMusic
		virtual void Reset();
		virtual BOOL Update(f32 dt);
		virtual const void *GetData() const;

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

		// IResouce
		virtual BOOL Load(const char *filename, ResourceManager *res = pResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

	private:
		SEED_DISABLE_COPY(Music);

	private:
		u32				eFormat;
		BOOL			bLoop;
		File			stFile;
		void			*pAVPlayer;
};

}} // namespace

#else // _IPHONE_
	#error "Include 'Music.h' instead 'platform/iphone/IphMusic.h' directly."
#endif // _IPHONE_
#endif // __IPHONE_MUSIC_H__
