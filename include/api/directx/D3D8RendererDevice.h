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

/*! \file D3D8RendererDevice.h
	\author	Danny Angelo Carminati Grein
	\brief DirectX 8.1 renderer device implementation
*/

#ifndef __D3D8_RENDERER_DEVICE_H__
#define __D3D8_RENDERER_DEVICE_H__

#include "Defines.h"
#include "Enum.h"
#include "Vertex.h"

#if defined(SEED_ENABLE_D3D8)

#include "interface/IRendererDevice.h"

#pragma push_macro("Delete")
#pragma push_macro("BOOL")
#pragma push_macro("SIZE_T")
#undef Delete
#undef BOOL
#undef SIZE_T
#include <dx8/d3d8.h>
#include <dx8/d3dx8.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("BOOL")
#pragma pop_macro("Delete")

#define D3DFVF_VERTEXFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace Seed {

class ITexture;

namespace DirectX {

class SEED_CORE_API D3D8RendererDevice : public IRendererDevice
{
	friend class IScreen;

	public:
		D3D8RendererDevice();
		virtual ~D3D8RendererDevice();

		virtual void Begin() const;
		virtual void End() const;

		// IRendererDevice
		virtual void TextureUnload(ITexture *texture);
		virtual void TextureRequest(ITexture *texture, void **texName);
		virtual void TextureRequestAbort(ITexture *texture, void **texName);
		virtual void TextureRequestProcess() const;
		virtual void TextureDataUpdate(ITexture *texture);

		virtual void SetBlendingOperation(eBlendMode mode, PIXEL color) const;
		virtual void UploadData(void *userData);
		virtual void BackbufferClear(const PIXEL color = 0);
		virtual void BackbufferFill(const PIXEL color = 0);

		virtual void SetViewport(const Rect<f32> &area) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill = FALSE) const;
		virtual void Enable2D() const;
		virtual void Disable2D() const;

		virtual void Update();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		mutable Array<ITexture *, 128> arTexture;
		mutable Array<void **, 128> arTextureName;

	private:
		SEED_DISABLE_COPY(D3D8RendererDevice);
		BOOL CheckExtension(const char *extName);

		D3DCAPS8				mCaps;
		LPDIRECT3D8				mObject;
		LPDIRECT3DDEVICE8		mDevice;
		D3DPRESENT_PARAMETERS	mParams;
		D3DDISPLAYMODE			mMode;

		BOOL					bLost;
		//LPDIRECT3DVERTEXBUFFER8 pVertexBuffer;
		//IDirect3DTexture8		*iTextureId;
};

}} // namespace

#else // SEED_ENABLE_D3D8
	#error "Include 'RendererDevice.h' instead 'api/directx/D3D8RendererDevice.h' directly."
#endif // SEED_ENABLE_D3D8

#endif // __D3D8_RENDERER_DEVICE_H__
