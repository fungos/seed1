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

#include "Defines.h"
//#include "Formats.h"
#include "extra/LZF/tgs_lzf.h"
#include "Log.h"

#define BLOCKSIZE (1024 * 64 - 1)
#define MAX_BLOCKSIZE BLOCKSIZE
#define TYPE0_HDR_SIZE 5
#define TYPE1_HDR_SIZE 7
#define MAX_HDR_SIZE 7
#define MIN_HDR_SIZE 5

struct myhead
{
	int type;
	int compression;
	u32 uncompressed_size;
};

u32 tgs_lzf_getuncompressedsize(const void *const in_data)
{
	struct myhead *myHeader = (struct myhead *)(in_data);

	return myHeader->uncompressed_size;
}

BOOL tgs_lzf_iscompressed(const void *const in_data)
{
	struct myhead *myHeader = (struct myhead *)(in_data);
	//if (myHeader->type != COMPRESSED_OBJECT_MAGIC)
		//return FALSE;

	return myHeader->compression == 1;

}

/*unsigned int tgs_lzf_decompress(const void *const in_data,  unsigned int in_len, void *out_data, unsigned int out_len)
{
	struct myhead *myHeader = const_cast<struct myhead *>(in_data);

	u8 *pInData = const_cast<u8 *>(in_data);
	void *ptr = static_cast<void *>(&pInData[sizeof(struct myhead)]);

	if (myHeader->compression == 1)
	{
		return lzf_decompress(ptr, in_len - sizeof(struct myhead), out_data, out_len);
	}

	out_data = ptr;
	return 1;
}*/

static size_t iterator = 0;

static inline size_t read (void *from, size_t fromLen, void *to, size_t len)
{
	if ((len + iterator) >= fromLen)
		len = (fromLen - iterator);

	if (len < 0)
		return (size_t)-1;

	MEMCOPY(to, from, (size_t)len);
	iterator += len;
	return len;
}

static inline size_t rread (void *from, size_t fromLen, void *to, size_t len)
{
	size_t rc = 0, offset = 0;
	char *p = (char *)(to);

	while (len && (rc = read(from, fromLen, &p[offset], len)) > 0)
	{
		offset += rc;
		len -= rc;
	}

	//nr_read += offset;

	if (rc < 0)
		return rc;

	return offset;
}

unsigned int tgs_lzf_decompress(const void *const in_data,  unsigned int in_len, void *out_data, unsigned int out_len)
{
	UNUSED(out_len);

	u8 header[MAX_HDR_SIZE];
	u8 buf1[MAX_BLOCKSIZE + MAX_HDR_SIZE + 16];
	u8 buf2[MAX_BLOCKSIZE + MAX_HDR_SIZE + 16];
	u8 *p = NULL;
	u8 *outdata = (u8 *)out_data;
	int l = 0, rd = 0, cs = 0, rc = 0; // cs, rc were size_t - DANNY
	size_t us = 0, bytes = 0, over = 0;

	//size_t dataWritten = 0;

	u8 *from = (u8 *)(in_data);

	struct myhead myHeader;
	rread (&from[iterator], in_len, (void *)(&myHeader), sizeof(struct myhead));

	//nr_read = nr_written = 0;
	while (1)
	{
		rc = rread (&from[iterator], in_len, header + over, MAX_HDR_SIZE - over);
		if (rc < 0)
		{
			Log("read error: ");
			ASSERT_NULL(0);
		}

		rc += over;
		over = 0;
		if (!rc || header[0] == 0)
			return 0;

		if (rc < MIN_HDR_SIZE || header[0] != 'Z' || header[1] != 'V')
		{
			Log("invalid data stream - magic not found or short header");
			ASSERT_NULL(0);
		}

		switch (header[2])
		{
			case 0:
				cs = (size_t)-1;
				us = (size_t)(header[3] << 8) | header[4];
				p = &header[TYPE0_HDR_SIZE];
			break;

			case 1:
				if (rc < TYPE1_HDR_SIZE)
				{
					Log("short data");
					ASSERT_NULL(0);
				}
				cs = (size_t)(header[3] << 8) | header[4];
				us = (size_t)(header[5] << 8) | header[6];
				p = &header[TYPE1_HDR_SIZE];
			break;

			default:
				Log("unknown blocktype");
				ASSERT_NULL(0);
			break;
		}

		bytes = cs == -1 ? us : cs;
		l = (int)(&header[rc] - p);

		if (l > 0)
			MEMCOPY(buf1, p, (size_t)l);

		if (l > (int)bytes)
		{
			over = l - bytes;
			memmove (header, &p[bytes], over);
		}

		p = &buf1[l];
		rd = (int)(bytes - l);
		if (rd > 0)
		{
			if ((rc = rread (&from[iterator], in_len, p, (size_t)rd)) != rd)
			{
				Log("short data");
				ASSERT_NULL(0);
			}
		}

		if (cs == -1)
		{
			MEMCOPY(outdata, buf1, us);
			outdata += us;
		}
		else
		{
			if (lzf_decompress(buf1, (unsigned int)cs, buf2, (unsigned int)us) != us)
			{
				Log("decompress: invalid stream - data corrupted");
				ASSERT_NULL(0);
			}

			MEMCOPY(outdata, buf2, us);
			outdata += us;
		}
	}
}
