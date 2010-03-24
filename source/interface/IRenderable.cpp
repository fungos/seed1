/*! \file IRenderable.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderable object interface
*/

#include "interface/IRenderable.h"
#include "Enum.h"
#include "Log.h"

namespace Seed {

IRenderable::IRenderable()
	: IObject()
	, eBlendOperation(NONE)
	, iPriority(0)
	, iColor(0)
	, bPersistent(FALSE)
	, bVisible(TRUE)
	, bMask(FALSE)
	, bMasked(FALSE)
	, bSpecial(FALSE)
	, fPriority(0.0f)
{
}

IRenderable::~IRenderable()
{
	this->Reset();
}

INLINE void IRenderable::Reset()
{
	this->eBlendOperation = NONE;
	this->iPriority = 0;
	this->iColor = 0;
	this->bPersistent = FALSE;
	this->bVisible = TRUE;
	this->bMask = FALSE;
	this->bMasked = FALSE;
	this->bSpecial = FALSE;
}

INLINE void IRenderable::Render(f32 delta)
{
	UNUSED(delta);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderable::Update(f32 delta)
{
	UNUSED(delta);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderable::SetPriority(u32 prio)
{
	iPriority = prio;
	fPriority = static_cast<f32>(prio);
}

INLINE u32 IRenderable::GetPriority() const
{
	return iPriority;
}

INLINE void IRenderable::SetPersistent(BOOL b)
{
	this->bPersistent = b;
}

INLINE BOOL IRenderable::IsPersistent() const
{
	return this->bPersistent;
}

INLINE void IRenderable::SetBlending(IRenderable::eBlendMode op)
{
	eBlendOperation = op;
}

INLINE void IRenderable::SetVisible(BOOL b)
{
	this->bVisible = b;
}

INLINE BOOL IRenderable::IsVisible() const
{
	return this->bVisible;
}

INLINE void IRenderable::SetMask(BOOL b)
{
	this->bMask = b;
}

INLINE BOOL IRenderable::IsMask() const
{
	return this->bMask;
}

INLINE void IRenderable::SetMasked(BOOL b)
{
	this->bMasked = b;
}

INLINE BOOL IRenderable::IsMasked() const
{
	return this->bMasked;
}

INLINE void IRenderable::SetSpecial(BOOL b)
{
	this->bSpecial = b;
}

INLINE BOOL IRenderable::IsSpecial() const
{
	return this->bSpecial;
}

INLINE void IRenderable::SetColor(u8 r, u8 g, u8 b, u8 a)
{
	this->iColor = PIXEL_COLOR(r, g, b, a);
}

INLINE void IRenderable::SetColor(f32 r, f32 g, f32 b, f32 a)
{
	this->iColor = PIXEL_COLOR(static_cast<u8>(r * 255), static_cast<u8>(g * 255), static_cast<u8>(b * 255), static_cast<u8>(a * 255));
}

INLINE void IRenderable::SetColor(PIXEL px)
{
	this->iColor = px;
}

INLINE PIXEL IRenderable::GetColor() const
{
	return this->iColor;
}

INLINE const char *IRenderable::GetObjectName() const
{
	return "IRenderable";
}

INLINE int IRenderable::GetObjectType() const
{
	return Seed::ObjectInterfaceRenderable;
}

} // namespace

