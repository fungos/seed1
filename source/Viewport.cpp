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

/*! \file Viewport.h
	\author	Everton Fernando Patitucci da Silva
	\brief Viewport implementation.
*/

#include "Viewport.h"
#include "Log.h"
#include "Renderer.h"
#include "RendererDevice.h"

namespace Seed {

Viewport::Viewport()
	: cArea(0.0f, 0.0f, 1.0f, 1.0f)
	, pRenderer(NULL)
{
}

Viewport::~Viewport()
{
}

INLINE void Viewport::SetRenderer(Renderer *renderer)
{
	ASSERT_NULL(renderer);

	pRenderer = renderer;
}

INLINE void Viewport::SetPosition(f32 x, f32 y)
{
	this->cArea.x = x;
	this->cArea.y = y;
}

INLINE void Viewport::SetWidth(f32 w)
{
	this->cArea.width = w;
}

INLINE void Viewport::SetHeight(f32 h)
{
	this->cArea.height = h;
}

INLINE Renderer *Viewport::GetRenderer() const
{
	return pRenderer;
}

INLINE f32 Viewport::GetX() const
{
	return cArea.x;
}

INLINE f32 Viewport::GetY() const
{
	return cArea.y;
}

INLINE f32 Viewport::GetWidth() const
{
	return cArea.width;
}

INLINE f32 Viewport::GetHeight() const
{
	return cArea.height;
}

INLINE void Viewport::Render()
{
	if (pRenderer)
	{
		pRendererDevice->SetViewport(cArea);
		pRenderer->Render();
	}
}

INLINE BOOL Viewport::Contains(f32 x, f32 y)
{
	return cArea.Contains(x, y);
}

INLINE const char *Viewport::GetObjectName() const
{
	return "Viewport";
}

} // namespace
