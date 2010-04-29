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

#if defined(_SDL_) || defined(_QT_)

#define OV_EXCLUDE_STATIC_CALLBACKS
#include "api/oal_ogg/vorbis_util.h"

size_t vorbis_read(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource)
{
	size_t spaceToEOF;
	size_t actualSizeToRead;
	sOggFile *vorbisData;

	vorbisData = (sOggFile *)datasource;

	spaceToEOF = static_cast<size_t>(vorbisData->dataSize - vorbisData->dataRead);
	if ((sizeToRead*byteSize) < spaceToEOF)
		actualSizeToRead = (sizeToRead*byteSize);
	else
		actualSizeToRead = spaceToEOF;

	if (actualSizeToRead)
	{
		memcpy(ptr, (char*)vorbisData->dataPtr + vorbisData->dataRead, actualSizeToRead);
		vorbisData->dataRead += (actualSizeToRead);
	}

	return actualSizeToRead;
}


int vorbis_seek(void *datasource, ogg_int64_t offset, int whence)
{
	size_t spaceToEOF;
	ogg_int64_t actualOffset;
	sOggFile *vorbisData;

	vorbisData = (sOggFile *)datasource;

	switch (whence)
	{
		case SEEK_SET:
		{
			if (vorbisData->dataSize >= offset)
				actualOffset = offset;
			else
				actualOffset = vorbisData->dataSize;
			vorbisData->dataRead = (int)actualOffset;
		}
		break;

		case SEEK_CUR:
		{
			spaceToEOF = static_cast<size_t>(vorbisData->dataSize - vorbisData->dataRead);
			if (offset < (ogg_int64_t)spaceToEOF)
				actualOffset = (offset);
			else
				actualOffset = spaceToEOF;
			vorbisData->dataRead += actualOffset;
		}
		break;

		case SEEK_END:
		{
			vorbisData->dataRead = vorbisData->dataSize + 1;
		}
		break;

		default:
		break;
	};

	return 0;
}


int vorbis_close(void *datasource)
{
	UNUSED(datasource);
	return 1;
}


long vorbis_tell(void *datasource)
{
	sOggFile *vorbisData = (sOggFile *)datasource;
	return static_cast<long>(vorbisData->dataRead);
}


bool ogg_update_stream(OggVorbis_File *oggStream, ogg_int64_t rate, ALenum format, ALuint buffer, bool loop)
{
	char pcm[VORBIS_BUFFER_SIZE];
	int size = 0;
	int section;
	int result;

	while (size < VORBIS_BUFFER_SIZE)
	{
		result = ov_read(oggStream, pcm + size, VORBIS_BUFFER_SIZE - size, 0, 2, 1, &section);

		if (result > 0)
			size += result;
		else
			if (result < 0)
				return false;
			else // end of file
			{
				if (loop)
					ov_raw_seek(oggStream, 0);
				else
					break;
			}

	}

	if (size == 0)
	{
		ov_raw_seek(oggStream, 0);
		return false;
	}

	alBufferData(buffer, format, pcm, size, static_cast<ALsizei>(rate));

	return true;
}

#endif // _SDL_ || _QT_
