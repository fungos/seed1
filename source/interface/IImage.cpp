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

/*! \file IImage.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Image class interface
*/

#include "interface/IImage.h"
#include "Enum.h"
#include "Log.h"

namespace Seed {

IImage::IImage()
	: nMinFilter(Seed::TextureFilterLinear)
	, nMagFilter(Seed::TextureFilterNearest)
	, iWidth(0)
	, iHeight(0)
	, fWidth(0.0f)
	, fHeight(0.0f)
{
}

IImage::~IImage()
{
	iWidth = 0;
	iHeight = 0;

	fWidth = 0.0f;
	fHeight = 0.0f;
}

INLINE const void *IImage::GetData() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

INLINE void IImage::PutPixel(u32 x, u32 y, PIXEL px)
{
	UNUSED(x)
	UNUSED(y)
	UNUSED(px)

	SEED_ABSTRACT_METHOD;
}

INLINE PIXEL IImage::GetPixel(u32 x, u32 y) const
{
	UNUSED(x)
	UNUSED(y)

	SEED_ABSTRACT_METHOD
	return 0;
}

INLINE u8 IImage::GetPixelAlpha(u32 x, u32 y) const
{
	UNUSED(x)
	UNUSED(y)

	SEED_ABSTRACT_METHOD
	return 0;
}

INLINE u32 IImage::GetAtlasWidthInPixel() const
{
	return iWidth;
}

INLINE u32 IImage::GetAtlasHeightInPixel() const
{
	return iHeight;
}

INLINE u32 IImage::GetWidthInPixel() const
{
	return iWidth;
}

INLINE u32 IImage::GetHeightInPixel() const
{
	return iHeight;
}

INLINE f32 IImage::GetWidth() const
{
	return fWidth;
}

INLINE f32 IImage::GetHeight() const
{
	return fHeight;
}

INLINE BOOL IImage::Unload()
{
	SEED_ABSTRACT_METHOD
	return TRUE;
}

INLINE BOOL IImage::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(filename)
	UNUSED(res)
	UNUSED(pool)

	SEED_ABSTRACT_METHOD
	return TRUE;
}

INLINE BOOL IImage::Load(u32 width, u32 height, PIXEL *buffer, IMemoryPool *pool)
{
	UNUSED(width)
	UNUSED(height)
	UNUSED(buffer)
	UNUSED(pool)

	SEED_ABSTRACT_METHOD
	return TRUE;
}

INLINE void IImage::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin)
		nMinFilter = filter;
	else
		nMagFilter = filter;
}

INLINE void IImage::Update()
{
	SEED_ABSTRACT_METHOD
}

INLINE int IImage::GetObjectType() const
{
	return Seed::ObjectImage;
}

INLINE const char *IImage::GetObjectName() const
{
	return "IImage";
}

SEED_DISABLE_INSTANCING_IMPL(IImage);

} // namespace
