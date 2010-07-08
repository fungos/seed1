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

/*! \file D3D8RendererDevice.cpp
	\author	Danny Angelo Carminati Grein
	\brief DirectX 8.1 renderer device implementation
*/

#include "RendererDevice.h"

#if defined(SEED_ENABLE_D3D8)

#include "Log.h"
#include "SeedInit.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "Texture.h"
#include "System.h"
#include "RendererManager.h"
#include "api/directx/DirectXVersion.h"

#if defined(_SDL_)
#include "platform/sdl/SdlDefines.h"
#endif

#define TAG "[D3D8RendererDevice] "
#define DXVER 0x00080001

namespace Seed { namespace DirectX {

D3D8RendererDevice::D3D8RendererDevice()
	: bLost(FALSE)
{
	Log(TAG "Initializing...");

	arTexture.Truncate();
	arTextureName.Truncate();

	Log(TAG "Initialization completed.");
}

D3D8RendererDevice::~D3D8RendererDevice()
{
}

INLINE BOOL D3D8RendererDevice::Initialize()
{
	BOOL ret = IRendererDevice::Initialize();

	Info(TAG "Initializing DirectX 8.1...");
	DWORD ver;
	TCHAR sver[1024];
	GetDXVersion(&ver, sver, 1024);

	HWND hWnd = (HWND)pScreen->iHandle;
	if (ver < DXVER)
	{
		char buf[1024];
		_vsnprintf(buf, 1024, "Direct3D 8.1 needed, current installed version: %s.", sver);

		MessageBox(hWnd, buf, "Wrong DirectX version", MB_OK);
		//pSystem->Shutdown();
		return FALSE;
	}
	//setting parameters that will be needed to created direct3D device

	mObject = Direct3DCreate8(D3D_SDK_VERSION);
	if (mObject == NULL)
	{
		MessageBox(hWnd, "Could not create Direct3D 8.1 Object", "Direct3D Object", MB_OK);
		//pSystem->Shutdown();
		return FALSE;
	}

	if (FAILED(mObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mMode)))
	{
		return FALSE;
	}

	ZeroMemory(&mParams, sizeof(mParams));
	mParams.Windowed = !pScreen->IsFullscreen();
	mParams.hDeviceWindow = hWnd;
	mParams.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
	mParams.BackBufferCount = 1;
	mParams.BackBufferWidth = pScreen->GetWidth();
	mParams.BackBufferHeight = pScreen->GetHeight();
	
	if (pScreen->IsFullscreen())
	{
		mParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		mParams.FullScreen_PresentationInterval  = D3DPRESENT_INTERVAL_ONE;
		mParams.BackBufferFormat = D3DFMT_R5G6B5;
	}
	else
	{
		mParams.BackBufferFormat = mMode.Format;
	}

	mParams.EnableAutoDepthStencil = FALSE;
	mParams.MultiSampleType = D3DMULTISAMPLE_NONE;

	int retry = 0;
	int error = 0;

	Info(TAG "Creating DirectX 8.1 HAL device...");
	for (retry = 0; retry < 10; retry++)
	{
		error = mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &mParams, &mDevice);
		if (FAILED(error))
		{
			Info(TAG "%d ERROR: Could not create DirectX HAL device (0x%x)", retry + 1, error);
			Sleep(100);
		}
		else break;
	}

	if (FAILED(error))
	{
		Info(TAG "Creating DirectX 8.1 REF device...");
		for (retry = 0; retry < 10; retry++)
		{
			error = mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &mParams, &mDevice);
			if (FAILED(error))
			{
				Info(TAG "%d ERROR: Could not create DirectX REF device (0x%x)", retry + 1, error);
				Sleep(100);
			}
			else break;
		}
	}

	if (FAILED(error))
	{
		char a[250];
		_snprintf(a, 250, "Error: %x", error);
		MessageBox(hWnd,"Could not create Direct3D 8.1 Device", a, MB_OK);
		//pSystem->Shutdown();
		return FALSE;
	}

	if (!pScreen->IsFullscreen())
	{
		RECT rcTmp,rectWindow;
		GetClientRect(hWnd, &rcTmp);
		rectWindow.left = 0;
		rectWindow.top = 0;
		rectWindow.right = rcTmp.right;
		rectWindow.bottom = rcTmp.bottom;

		SetWindowPos(hWnd, HWND_TOP, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom, SWP_SHOWWINDOW);
	}

	D3DVIEWPORT8 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = pScreen->GetWidth();
	vp.Height = pScreen->GetHeight();
	vp.MinZ = 0.0;
	vp.MaxZ = 0.0f;
	mDevice->SetViewport(&vp);

	mDevice->GetDeviceCaps(&mCaps);
	Info(TAG "DirectX 8.1 Capabilities:");
	Info(TAG "\tMaxTextureAspectRatio: %d", mCaps.MaxTextureAspectRatio);
	Info(TAG "\tMaxPrimitiveCount: %d", mCaps.MaxPrimitiveCount);

	/*
	Maximum number of primitives for each DrawPrimitive  call. Note that when Direct3D is working with a
	DirectX 6.0 or DirectX 7.0 driver, this field is set to 0xFFFF. This means that not only the number of primitives
	but also the number of vertices is limited by this value. 
	*/
	Info(TAG "\tMaxVertexIndex: %d", mCaps.MaxVertexIndex);
	/*
	Maximum size of indices supported for hardware vertex processing. It is possible to create 32-bit index
	buffers by specifying D3DFMT_INDEX32; however, you will not be able to render with the index buffer unless
	this value is greater than 0x0000FFFF.
	*/

	//mDevice->CreateVertexBuffer(4 * sizeof(sVertex), D3DUSAGE_WRITEONLY, D3DFVF_VERTEXFORMAT, D3DPOOL_MANAGED, &pVertexBuffer);
	//mDevice->CreateTexture(1024, 1024, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &iTextureId);

	this->Enable2D();

	return ret;
}

INLINE BOOL D3D8RendererDevice::Reset()
{
	arTexture.Truncate();
	arTextureName.Truncate();

	HWND hWnd = (HWND)pScreen->iHandle;
	ZeroMemory(&mParams, sizeof(mParams));
	mParams.Windowed = !pScreen->IsFullscreen();
	mParams.hDeviceWindow = hWnd;
	mParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mParams.BackBufferCount = 1;
	mParams.BackBufferWidth = pScreen->GetWidth();
	mParams.BackBufferHeight = pScreen->GetHeight();
	
	if (pScreen->IsFullscreen())
	{
		mParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		mParams.FullScreen_PresentationInterval  = D3DPRESENT_INTERVAL_ONE;
		mParams.BackBufferFormat = D3DFMT_R5G6B5;
	}
	else
	{
		mParams.BackBufferFormat = mMode.Format;
	}

	mParams.EnableAutoDepthStencil = FALSE;
	mParams.MultiSampleType = D3DMULTISAMPLE_NONE;

	mDevice->Reset(&mParams);

	this->Enable2D();

	return TRUE; // abstract IRenderer::Reset();
}

INLINE BOOL D3D8RendererDevice::Shutdown()
{
	BOOL ret = IRendererDevice::Shutdown();

	arTexture.Truncate();
	arTextureName.Truncate();

	int objects = 0;
	if (mDevice)
	{
		if ((objects = mDevice->Release()) > 0)
		{
			Info(TAG "There are %d objects in RendererDevice - shutdown will fail.", objects);
			ret = FALSE;
		}
	}
	mDevice = NULL;

	if (mObject)
		mObject->Release();
	mObject = NULL;

	return ret;
}

INLINE void D3D8RendererDevice::BackbufferClear(const PIXEL color)
{
	uPixel p(0, 0, 0, 0);
	p.pixel = color;
	mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(p.argb.a, p.argb.r, p.argb.g, p.argb.b), 0.0f, 0);
}

INLINE void D3D8RendererDevice::Begin() const
{
	this->TextureRequestProcess();

	mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0,0,0), 0.0f, 0);
	mDevice->BeginScene();
}

INLINE void D3D8RendererDevice::End() const
{
	pScreen->ApplyFade();
	mDevice->EndScene();
	mDevice->Present(NULL, NULL, NULL, NULL);
}

INLINE void D3D8RendererDevice::SetBlendingOperation(eBlendMode mode, PIXEL color) const
{
	UNUSED(color);
	switch (mode)
	{
		default:
		case Seed::BlendNone:
		{
			mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			mDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		}
		break;

		case Seed::BlendAdditive:
		{
			mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			mDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
		break;

		case Seed::BlendModulate:
		{
			mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			mDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			mDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			mDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

			mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		}
		break;

		case Seed::BlendModulateAlpha:
		{
			mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			mDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
		break;

		case Seed::BlendColorDodge:
		case Seed::BlendDecalOverlay:
		case Seed::BlendLighten:
		case Seed::BlendMerge:
		case Seed::BlendOverlay:
		case Seed::BlendScreen:
		{
			Dbg(TAG "Blending operation not implemented.");
		}
		break;
	}
}

INLINE void D3D8RendererDevice::TextureRequestAbort(ITexture *texture, void **texName)
{
	arTexture.Remove(texture);
	arTextureName.Remove(texName);
}

INLINE void D3D8RendererDevice::TextureRequest(ITexture *texture, void **texName)
{
	arTexture.Add(texture);
	arTextureName.Add(texName);
}

INLINE void D3D8RendererDevice::TextureRequestProcess() const
{
	for (u32 i = 0; i < arTexture.Size(); i++)
	{
		ITexture *texture = arTexture[i];
		IDirect3DTexture8 **tex = (IDirect3DTexture8 **)arTextureName[i];

		File *file = texture->GetFile();
		if (file->GetData())
		{
			D3DXCreateTextureFromFileInMemory(mDevice, file->GetData(), file->GetSize(), tex);
		}
		else
		{
			u32 w = texture->GetAtlasWidthInPixel();
			u32 h = texture->GetAtlasHeightInPixel();

			// FIXME: only 32bits for now
			mDevice->CreateTexture(w, h, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, tex);
			IDirect3DTexture8 *t = *tex;

			const void *data = texture->GetData();
			if (t && data)
			{
				D3DLOCKED_RECT r;
				t->LockRect(0, &r, NULL, D3DLOCK_DISCARD);
				if (r.pBits)
				{
					MEMCOPY(r.pBits, data, w * h * 4);
				}
				t->UnlockRect(0);
			}
		}

		texture->Close();
	}

	arTexture.Truncate();
	arTextureName.Truncate();
}

INLINE void D3D8RendererDevice::TextureUnload(ITexture *texture)
{
	void *texId = texture->GetTextureName();
	if (texId)
	{
		IDirect3DTexture8 *t = static_cast<IDirect3DTexture8 *>(texId);
		t->Release();
	}
}

INLINE void D3D8RendererDevice::TextureDataUpdate(ITexture *texture)
{
	const void *data = texture->GetData();
	if (data)
	{
		u32 w = texture->GetAtlasWidthInPixel();
		u32 h = texture->GetAtlasHeightInPixel();
		IDirect3DTexture8 *t = static_cast<IDirect3DTexture8 *>(texture->GetTextureName());

		D3DLOCKED_RECT r;
		t->LockRect(0, &r, NULL, D3DLOCK_DISCARD);
		if (r.pBits)
		{
			MEMCOPY(r.pBits, data, w * h * 4);
		}
		t->UnlockRect(0);
	}
}

INLINE void D3D8RendererDevice::UploadData(void *userData)
{
	RendererPacket *packet = static_cast<RendererPacket *>(userData);

//	this->SetBlendingOperation(packet->nBlendMode, packet->iColor.pixel);

	ITexture *texture = packet->pTexture;
	IDirect3DTexture8 *t = static_cast<IDirect3DTexture8 *>(texture->GetTextureName());

	mDevice->SetTexture(0, t);
	mDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	mDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

	eTextureFilter min = texture->GetFilter(Seed::TextureFilterTypeMin);
	eTextureFilter mag = texture->GetFilter(Seed::TextureFilterTypeMag);

	if (min == Seed::TextureFilterLinear)
		mDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	if (min == Seed::TextureFilterNearest)
		mDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);

	if (mag == Seed::TextureFilterLinear)
		mDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	if (mag == Seed::TextureFilterNearest)
		mDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);

	mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, packet->iSize - 2, packet->pVertexData, sizeof(sVertex));
}

void D3D8RendererDevice::BackbufferFill(PIXEL color)
{
	uPixel argb;
	argb.pixel = color;
	uPixel rgba = argb;
	rgba.rgba.r = argb.argb.r;
	rgba.rgba.g = argb.argb.g;
	rgba.rgba.b = argb.argb.b;
	rgba.rgba.a = argb.argb.a;

	sVertex quad[4];

	quad[0].cVertex.x = 0.0f;
	quad[0].cVertex.y = 0.0f;
	quad[0].cVertex.z = 1.0f;
	quad[0].iColor = rgba;

	quad[1].cVertex.x = 1.0f;
	quad[1].cVertex.y = 0.0f;
	quad[1].cVertex.z = 1.0f;
	quad[1].iColor = rgba;

	quad[2].cVertex.x = 0.0f;
	quad[2].cVertex.y = 1.0f;
	quad[2].cVertex.z = 1.0f;
	quad[2].iColor = rgba;

	quad[3].cVertex.x = 1.0f;
	quad[3].cVertex.y = 1.0f;
	quad[3].cVertex.z = 1.0f;
	quad[3].iColor = rgba;

	this->SetBlendingOperation(Seed::BlendModulate, 0);
	mDevice->SetTexture(0, NULL);

	mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &quad, sizeof(sVertex));
}

void D3D8RendererDevice::SetViewport(const Rect<f32> &area) const
{
	u32 x = static_cast<u32>(area.x * pScreen->GetWidth());
	u32 y = static_cast<u32>(area.y * pScreen->GetHeight());
	u32 width = static_cast<u32>(area.width * pScreen->GetWidth());
	u32 height = static_cast<u32>(area.height * pScreen->GetHeight());

	D3DVIEWPORT8 vp;
	vp.X = x;
	vp.Y = pScreen->GetHeight() - y - height;
	vp.Width = width;
	vp.Height = height;
	vp.MinZ = 0.0;
	vp.MaxZ = 0.0f;
	mDevice->SetViewport(&vp);
}

INLINE void D3D8RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	UNUSED(fill);

	uPixel argb;
	argb.pixel = color;
	uPixel rgba = argb;
	rgba.rgba.r = argb.argb.r;
	rgba.rgba.g = argb.argb.g;
	rgba.rgba.b = argb.argb.b;
	rgba.rgba.a = argb.argb.a;

	f32 ratio = ((f32)pScreen->GetHeight() / (f32)pScreen->GetWidth());

	sVertex quad[5];

	quad[0].cVertex.x = x;
	quad[0].cVertex.y = y * ratio;
	quad[0].cVertex.z = 1.0f;
	quad[0].iColor = rgba;

	quad[1].cVertex.x = x + w;
	quad[1].cVertex.y = y * ratio;
	quad[1].cVertex.z = 1.0f;
	quad[1].iColor = rgba;

	quad[2].cVertex.x = x + w;
	quad[2].cVertex.y = (y + h) * ratio;
	quad[2].cVertex.z = 1.0f;
	quad[2].iColor = rgba;

	quad[3].cVertex.x = x;
	quad[3].cVertex.y = (y + h) * ratio;
	quad[3].cVertex.z = 1.0f;
	quad[3].iColor = rgba;

	quad[4].cVertex.x = x;
	quad[4].cVertex.y = y * ratio;
	quad[4].cVertex.z = 1.0f;
	quad[4].iColor = rgba;

	this->SetBlendingOperation(Seed::BlendModulate, 0);
	mDevice->SetTexture(0, NULL);

	mDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &quad, sizeof(sVertex));
}

// FIXME: Viewport aspect ratio...
INLINE void D3D8RendererDevice::Enable2D() const
{
	f32 viewW = static_cast<f32>(pScreen->GetWidth());
	f32 viewH = static_cast<f32>(pScreen->GetHeight());

	f32 aspectH = viewH / viewW;

	D3DXMATRIX matOrtho;
	D3DXMATRIX matIdentity;
	D3DXMATRIX matTrans;
	D3DXMATRIX matTmpOrtho;
	D3DXMatrixOrthoLH(&matTmpOrtho, 1.0f, -aspectH, 0.0f, (f32)SEED_MAX_PRIORITY);
	D3DXMatrixTranslation(&matTrans, -0.5f, -aspectH/2.0f, 0.0f);
	D3DXMatrixMultiply(&matOrtho, &matTrans, &matTmpOrtho);

	D3DXMatrixIdentity(&matIdentity);

	mDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	mDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	mDevice->SetTransform(D3DTS_VIEW, &matIdentity);

	mDevice->SetVertexShader(D3DFVF_VERTEXFORMAT);
	mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	mDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	mDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	mDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

INLINE void D3D8RendererDevice::Disable2D() const
{
	// not implemented
}

INLINE void D3D8RendererDevice::Update()
{
	if (mDevice)
	{
		switch (mDevice->TestCooperativeLevel())
		{
			case D3DERR_DEVICELOST:
			{
				if (!bLost)
				{
					Info(TAG "WARNING: DirectX Device Lost.");
					pRendererManager->Disable();
					bLost = TRUE;
				}
			}
			break;
			
			case D3DERR_DEVICENOTRESET:
			{
				if (bLost)
				{
					Info(TAG "WARNING: DirectX Device Reset.");

					pResourceManager->Unload(Seed::ObjectTexture);
					this->Shutdown();
					this->Initialize();
					pResourceManager->Reload(Seed::ObjectTexture);

					pRendererManager->Enable();
					bLost = FALSE;
				}
			}
			break;

			default:
			{
			}
			break;
		}
	}
}

INLINE const char *D3D8RendererDevice::GetObjectName() const
{
	return "D3D8RendererDevice";
}

}} // namespace

#endif // SEED_ENABLE_D3D8
