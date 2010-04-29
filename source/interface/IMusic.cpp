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

/*! \file IMusic.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Music class interface
*/

#include "interface/IMusic.h"

namespace Seed {

IMusic::IMusic()
	: fVolume(1.0f)
	, eState(Seed::MusicNone)
	, bAutoUnload(FALSE)
{
}

IMusic::~IMusic()
{
}

INLINE BOOL IMusic::Unload()
{
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

INLINE BOOL IMusic::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(filename);
	UNUSED(res);
	UNUSED(pool);
	SEED_ABSTRACT_METHOD;

	return FALSE;
}

INLINE BOOL IMusic::Update(f32 dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;

	return FALSE;
}

INLINE const void *IMusic::GetData() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE void IMusic::SetVolume(f32 volume)
{
	this->fVolume = volume;
}

INLINE f32 IMusic::GetVolume() const
{
	return this->fVolume;
}

INLINE void IMusic::SetAutoUnload(BOOL b)
{
	this->bAutoUnload = b;
}

INLINE int IMusic::GetObjectType() const
{
	return Seed::ObjectMusic;
}

INLINE const char *IMusic::GetObjectName() const
{
	return "IMusic";
}

INLINE void IMusic::Play()
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IMusic::Stop()
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IMusic::Pause()
{
	SEED_ABSTRACT_METHOD;
}

INLINE BOOL IMusic::IsPlaying() const
{
	return ((eState != MusicStopped) &&
			(eState != MusicStop) &&
			(eState != MusicPause) &&
			(eState != MusicPaused) &&
			(eState != MusicNone));
}

SEED_DISABLE_INSTANCING_IMPL(IMusic);

} // namespace
