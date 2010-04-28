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

#ifndef __VORBIS_UTIL_H__
#define __VORBIS_UTIL_H__

#if defined(_SDL_) || defined(_QT_)

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#include "Defines.h"

// 40kb para que em maquinas lentas nao fique engasgando no som
#define VORBIS_BUFFER_SIZE SEED_MUSIC_STREAMING_BUFFER_SIZE


struct SEED_CORE_API sOggFile
{
	u8		*dataPtr;
	u32		dataSize;
	u64		dataRead;

	sOggFile()
		: dataPtr(NULL)
		, dataSize(0)
		, dataRead(0)
	{}
};


size_t vorbis_read(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource);
int vorbis_seek(void *datasource, ogg_int64_t offset, int whence);
int vorbis_close(void *datasource);
long vorbis_tell(void *datasource);
bool ogg_update_stream(OggVorbis_File *oggStream, ogg_int64_t rate, ALenum format, ALuint buffer, bool loop);

#endif // _SDL_ || _QT_

#endif // __VORBIS_UTIL_H__
