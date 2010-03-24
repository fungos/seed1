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

INLINE void *IImage::operator new(size_t len)
{
	return pMemoryManager->Alloc(len, pDefaultPool);
}

INLINE void IImage::operator delete(void *ptr)
{
	pMemoryManager->Free(ptr, pDefaultPool);
}

INLINE int IImage::GetObjectType() const
{
	return Seed::ObjectImage;
}

INLINE const char *IImage::GetObjectName() const
{
	return "IImage";
}

} // namespace
