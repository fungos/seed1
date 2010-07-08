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

/*! \file ITexture.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Texture class interface
*/

#include "interface/IFileSystem.h"
#include "interface/ITexture.h"
#include "Enum.h"
#include "Log.h"

namespace Seed {

ITexture::ITexture()
	: stFile()
	, nMinFilter(Seed::TextureFilterLinear)
	, nMagFilter(Seed::TextureFilterNearest)
	, iWidth(0)
	, iHeight(0)
	, fWidth(0.0f)
	, fHeight(0.0f)
{
}

ITexture::~ITexture()
{
	this->Reset();
}

INLINE void ITexture::Reset()
{
	stFile.Close();

	iWidth = 0;
	iHeight = 0;

	fWidth = 0.0f;
	fHeight = 0.0f;

	nMinFilter = Seed::TextureFilterLinear;
	nMagFilter = Seed::TextureFilterNearest;
}

INLINE File *ITexture::GetFile()
{
	return &stFile;
}

INLINE const void *ITexture::GetData() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE void ITexture::PutPixel(u32 x, u32 y, PIXEL px)
{
	UNUSED(x)
	UNUSED(y)
	UNUSED(px)

	SEED_ABSTRACT_METHOD;
}

INLINE PIXEL ITexture::GetPixel(u32 x, u32 y) const
{
	UNUSED(x)
	UNUSED(y)

	SEED_ABSTRACT_METHOD
	return 0;
}

INLINE u8 ITexture::GetPixelAlpha(u32 x, u32 y) const
{
	UNUSED(x)
	UNUSED(y)

	SEED_ABSTRACT_METHOD
	return 0;
}

INLINE u32 ITexture::GetAtlasWidthInPixel() const
{
	return iWidth;
}

INLINE u32 ITexture::GetAtlasHeightInPixel() const
{
	return iHeight;
}

INLINE u32 ITexture::GetWidthInPixel() const
{
	return iWidth;
}

INLINE u32 ITexture::GetHeightInPixel() const
{
	return iHeight;
}

INLINE f32 ITexture::GetWidth() const
{
	return fWidth;
}

INLINE f32 ITexture::GetHeight() const
{
	return fHeight;
}

INLINE void ITexture::Close()
{
	stFile.Close();
}

INLINE BOOL ITexture::Unload()
{
	stFile.Close();

	return TRUE;
}

INLINE BOOL ITexture::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(pool);
	ASSERT_NULL(res);

	BOOL ret = FALSE;
	if (this->Unload())
	{
		pPool = pool;
		pRes = res;
		pFilename = filename;

		pFileSystem->Open(filename, &stFile, pool);

		ret = (stFile.GetData() != NULL);
	}

	return ret;
}

INLINE BOOL ITexture::Load(u32 width, u32 height, PIXEL *buffer, u32 atlasWidth, u32 atlasHeight, IMemoryPool *pool)
{
	UNUSED(width)
	UNUSED(height)
	UNUSED(buffer)
	UNUSED(pool)
	UNUSED(atlasWidth)
	UNUSED(atlasHeight)

	SEED_ABSTRACT_METHOD
	return TRUE;
}

INLINE void ITexture::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin)
		nMinFilter = filter;
	else
		nMagFilter = filter;
}

INLINE eTextureFilter ITexture::GetFilter(eTextureFilterType type) const
{
	eTextureFilter filter = nMinFilter;

	if (type == Seed::TextureFilterTypeMag)
		filter = nMagFilter;

	return filter;
}

INLINE u32 ITexture::GetBytesPerPixel() const
{
	return 0;
}

INLINE void *ITexture::GetTextureName() const
{
	return NULL;
}

INLINE void ITexture::Update(PIXEL *buffer)
{
	UNUSED(buffer);
	SEED_ABSTRACT_METHOD;
}

INLINE int ITexture::GetObjectType() const
{
	return Seed::ObjectTexture;
}

INLINE const char *ITexture::GetObjectName() const
{
	return "ITexture";
}

SEED_DISABLE_INSTANCING_IMPL(ITexture);

} // namespace
