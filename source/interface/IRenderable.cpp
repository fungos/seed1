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

/*! \file IRenderable.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderable object interface
*/

#include "interface/IRenderable.h"
#include "Enum.h"
#include "Log.h"

namespace Seed {

IRenderable::IRenderable()
	: eBlendOperation(BlendNone)
	, iColor(255, 255, 255, 255)
	, bVisible(TRUE)
{
}

IRenderable::~IRenderable()
{
	this->Reset();
}

INLINE void IRenderable::Reset()
{
	eBlendOperation = BlendNone;
	iColor.pixel = 0;
	bVisible = TRUE;
}

INLINE void IRenderable::Render()
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRenderable::Update(f32 delta)
{
	UNUSED(delta);
	SEED_ABSTRACT_METHOD;
}

/*
INLINE void IRenderable::SetPriority(u32 prio)
{
	iPriority = prio;
	fPriority = static_cast<f32>(prio);
}

INLINE u32 IRenderable::GetPriority() const
{
	return iPriority;
}
*/

INLINE void IRenderable::SetBlending(eBlendMode op)
{
	eBlendOperation = op;
}

INLINE void IRenderable::SetVisible(BOOL b)
{
	bVisible = b;
}

INLINE BOOL IRenderable::IsVisible() const
{
	return bVisible;
}

INLINE void IRenderable::SetColor(u32 r, u32 g, u32 b, u32 a)
{
	iColor.pixel = PIXEL_COLOR(r, g, b, a);
}

INLINE void IRenderable::SetColor(f32 r, f32 g, f32 b, f32 a)
{
	iColor.pixel = PIXEL_COLOR(static_cast<u32>(r * 255), static_cast<u32>(g * 255), static_cast<u32>(b * 255), static_cast<u32>(a * 255));
}

INLINE void IRenderable::SetColor(PIXEL px)
{
	iColor.pixel = px;
}

INLINE PIXEL IRenderable::GetColor() const
{
	return iColor.pixel;
}

} // namespace
