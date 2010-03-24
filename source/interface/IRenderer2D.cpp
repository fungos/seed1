/*! \file IRenderer2D.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderer 2D class interface
*/

#include "interface/IRenderer2D.h"
#include "Log.h"

namespace Seed {

IRenderer2D::IRenderer2D()
{
}

IRenderer2D::~IRenderer2D()
{
}

INLINE void IRenderer2D::ClearScreen(const PIXEL color) const
{
	UNUSED(color);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderer2D::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	UNUSED(color);
	UNUSED(fill);
	SEED_ABSTRACT_METHOD;
}

} // namespace
