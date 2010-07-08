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

/*! \file PcRendererDevice.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer device factory
*/

#include "RendererDevice.h"
#include "SeedInit.h"

#if defined(_PC_)

#define TAG "[RendererDevice] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(RendererDevice);

RendererDevice::RendererDevice()
	: IRendererDevice()
	, cNull()
	, pApiDevice(&cNull)
{
	cNull.Disable();
}

RendererDevice::~RendererDevice()
{
	if (pApiDevice != &cNull)
		Delete(pApiDevice);

	pApiDevice = NULL;
}

BOOL RendererDevice::Initialize()
{
	Log(TAG "Initializing...");

	switch (pConfiguration->GetRendererDeviceType())
	{
#if defined(SEED_ENABLE_OGL20)
		case RendererDeviceOpenGL20:
		{
			Info(TAG "Creating renderer device OpenGL 2.0");
			pApiDevice = New(OGL20RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGL30)
		case RendererDeviceOpenGL30:
		{
			Info(TAG "Creating renderer device OpenGL 3.0");
			pApiDevice = New(OGL30RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGL40)
		case RendererDeviceOpenGL40:
		{
			Info(TAG "Creating renderer device OpenGL 4.0");
			pApiDevice = New(OGL40RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D8)
		case RendererDeviceDirectX8:
		{
			Info(TAG "Creating renderer device DirectX 8.1");
			pApiDevice = New(Seed::DirectX::D3D8RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D9)
		case RendererDeviceDirectX9:
		{
			Info(TAG "Creating renderer device DirectX 9.0c");
			pApiDevice = New(D3D9RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D10)
		case RendererDeviceDirectX10:
		{
			Info(TAG "Creating renderer device DirectX 10");
			pApiDevice = New(D3D10RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D11)
		case RendererDeviceDirectX11:
		{
			Info(TAG "Creating renderer device DirectX 11");
			pApiDevice = New(D3D11RendererDevice());
		}
		break;
#endif

		case RendererDeviceWii:
		case RendererDeviceOpenGLES:
		case RendererDeviceOpenGL14:
		default:
		{
			Info(TAG "Creating renderer device OpenGL 1.4");
			pApiDevice = New(Seed::OpenGL::OGL14RendererDevice());
		}
		break;
	}

	BOOL ret = pApiDevice->Initialize();

	return ((pApiDevice != &cNull) && ret);
}

INLINE BOOL RendererDevice::Reset()
{
	return pApiDevice->Reset();
}

INLINE BOOL RendererDevice::Shutdown()
{
	BOOL ret = pApiDevice->Shutdown();
	Delete(pApiDevice);
	pApiDevice = &cNull;

	return ret;
}

INLINE void RendererDevice::TextureRequest(ITexture *texture, void **texName)
{
	pApiDevice->TextureRequest(texture, texName);
}

INLINE void RendererDevice::TextureRequestAbort(ITexture *texture, void **texName)
{
	pApiDevice->TextureRequestAbort(texture, texName);
}

INLINE void RendererDevice::TextureRequestProcess() const
{
	pApiDevice->TextureRequestProcess();
}

INLINE void RendererDevice::TextureUnload(ITexture *tex)
{
	pApiDevice->TextureUnload(tex);
}

INLINE void RendererDevice::TextureDataUpdate(ITexture *texture)
{
	pApiDevice->TextureDataUpdate(texture);
}

INLINE void RendererDevice::SetBlendingOperation(eBlendMode mode, PIXEL color) const
{
	pApiDevice->SetBlendingOperation(mode, color);
}

INLINE void RendererDevice::UploadData(void *userData)
{
	pApiDevice->UploadData(userData);
}

INLINE void RendererDevice::BackbufferClear(const PIXEL color)
{
	pApiDevice->BackbufferClear(color);
}

INLINE void RendererDevice::BackbufferFill(const PIXEL color)
{
	pApiDevice->BackbufferFill(color);
}

INLINE void RendererDevice::Begin() const
{
	pApiDevice->Begin();
}

INLINE void RendererDevice::End() const
{
	pApiDevice->End();
}

INLINE void RendererDevice::SetViewport(const Rect<f32> &area) const
{
	pApiDevice->SetViewport(area);
}

INLINE void RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	pApiDevice->DrawRect(x, y, w, h, color, fill);
}

INLINE void RendererDevice::Enable2D() const
{
	pApiDevice->Enable2D();
}

INLINE void RendererDevice::Disable2D() const
{
	pApiDevice->Disable2D();
}

INLINE void RendererDevice::Update()
{
	pApiDevice->Update();
}

}} // namespace

#endif // _PC_
