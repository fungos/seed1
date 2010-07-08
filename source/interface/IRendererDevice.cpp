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

/*! \file IRendererDevice.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer Device interfacve
*/

#include "interface/IRendererDevice.h"
#include "Log.h"

namespace Seed {

IRendererDevice::IRendererDevice()
{
}

IRendererDevice::~IRendererDevice()
{
}

INLINE void IRendererDevice::TextureUnload(ITexture *tex)
{
	UNUSED(tex);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::TextureDataUpdate(ITexture *tex)
{
	UNUSED(tex);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::SetBlendingOperation(eBlendMode mode, PIXEL color) const
{
	UNUSED(mode);
	UNUSED(color);

	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::UploadData(void *userData)
{
	UNUSED(userData);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::Begin() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::End() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::BackbufferClear(const PIXEL color)
{
	UNUSED(color);
	SEED_ABSTRACT_METHOD;
}

INLINE BOOL IRendererDevice::IsRequired() const
{
	return TRUE;
}

INLINE void IRendererDevice::TextureRequestAbort(ITexture *texture, void **texName)
{
	UNUSED(texture);
	UNUSED(texName);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::TextureRequest(ITexture *texture, void **texName)
{
	UNUSED(texture);
	UNUSED(texName);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::TextureRequestProcess() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::BackbufferFill(const PIXEL color)
{
	UNUSED(color);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::SetViewport(const Rect<f32> &area) const
{
	UNUSED(area);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	UNUSED(color);
	UNUSED(fill);
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::Enable2D() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::Disable2D() const
{
	SEED_ABSTRACT_METHOD;
}

INLINE void IRendererDevice::Update()
{
}

INLINE const char *IRendererDevice::GetObjectName() const
{
	return "IRendererDevice";
}

} // namespace
