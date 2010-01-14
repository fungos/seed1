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
{
}

IImage::~IImage()
{
}

INLINE const void *IImage::GetData() const
{
	SEED_ABSTRACT_METHOD(IImage::GetData);
	return NULL;
}

INLINE void IImage::PutPixel(u32 x, u32 y, PIXEL px)
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(px);

	SEED_ABSTRACT_METHOD(IImage::PutPixel);
}

INLINE PIXEL IImage::GetPixel(u32 x, u32 y) const
{
	UNUSED(x);
	UNUSED(y);

	SEED_ABSTRACT_METHOD(IImage::GetPixel);
	return 0;
}

INLINE u8 IImage::GetPixelAlpha(u32 x, u32 y) const
{
	UNUSED(x);
	UNUSED(y);

	SEED_ABSTRACT_METHOD(IImage::GetPixel);
	return 0;
}

INLINE u32 IImage::GetWidthInPixel() const
{
	SEED_ABSTRACT_METHOD(IImage::GetWidthInPixel);
	return 0;
}

INLINE u32 IImage::GetHeightInPixel() const
{
	SEED_ABSTRACT_METHOD(IImage::GetHeightInPixel);
	return 0;
}

INLINE f32 IImage::GetWidth() const
{
	SEED_ABSTRACT_METHOD(IImage::GetWidth);
	return 0.0f;
}

INLINE f32 IImage::GetHeight() const
{
	SEED_ABSTRACT_METHOD(IImage::GetHeight);
	return 0.0f;
}

INLINE BOOL IImage::Unload()
{
	SEED_ABSTRACT_METHOD(IImage::Unload);
	return TRUE;
}

INLINE BOOL IImage::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	UNUSED(filename);
	UNUSED(res);
	UNUSED(pool);

	SEED_ABSTRACT_METHOD(IImage::Load);
	return TRUE;
}

INLINE void IImage::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin)
		nMinFilter = filter;
	else
		nMagFilter = filter;
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

