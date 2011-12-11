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

/*! \file PS3Screen.cpp
	\author	Danny Angelo Carminati Grein
	\brief ps3dev screen implementation
*/

#if defined(_PS3DEV_)

#include "Screen.h"
#include "Log.h"
#include "SeedInit.h"
#include "RendererDevice.h"
#include <malloc.h>
#include <unistd.h>

#define TAG "[Screen] "

#define HOST_SIZE (32 * 1024 * 1024)

namespace Seed { namespace PS3 {

SEED_SINGLETON_DEFINE(Screen)

Screen::Screen()
	: pColorBuffer(NULL)
	, pContext(NULL)
	, surfaceSize(0)
	, cVideoDeviceInfo()
	, cVideoState()
	, cVideoConfig()
	, iCurBuffer(PS3_MAX_BUFFERS - 1)
	, iFadeStatus(0)
	, iBPP(32)
	, fadeType(FADE_IN)
	, iLabel(1)
	, arColorBuffers()
	, sDepthBuffer()
	, pHostAddr(NULL)
{
	pColorBuffer = &arColorBuffers[0];
}

Screen::~Screen()
{
	this->Reset();
}

INLINE BOOL Screen::Reset()
{
	return TRUE;
}

INLINE BOOL Screen::PrepareMode()
{
	videoGetDeviceInfo(0, 0, &cVideoDeviceInfo);

	videoColorInfo *ci = &cVideoDeviceInfo.colorInfo;

	Info(TAG "PS3 Video Device Info:");
	Info(TAG "\tPort Type............: %s (%d)", (cVideoDeviceInfo.portType == VIDEO_PORT_NONE ? "none" :
											cVideoDeviceInfo.portType == VIDEO_PORT_HDMI ? "hdmi" :
											cVideoDeviceInfo.portType == VIDEO_PORT_NETWORK ? "network" :
											cVideoDeviceInfo.portType == VIDEO_PORT_COMPOSITE ? "composite" :
											cVideoDeviceInfo.portType == VIDEO_PORT_D ? "D" :
											cVideoDeviceInfo.portType == VIDEO_PORT_COMPONENT ? "component" :
											cVideoDeviceInfo.portType == VIDEO_PORT_RGB ? "rgb" :
											cVideoDeviceInfo.portType == VIDEO_PORT_SCART ? "scart" : "dsub"), cVideoDeviceInfo.portType);
	Info(TAG "\tColor Space..........: %s (%d)", (cVideoDeviceInfo.colorSpace == VIDEO_COLOR_RGB ? "rgb" :
											cVideoDeviceInfo.colorSpace == VIDEO_COLOR_YUV ? "yuv" : "xvycc"), cVideoDeviceInfo.colorSpace);
	Info(TAG "\tLatency..............: %s (%d)", (cVideoDeviceInfo.latency == VIDEO_REFRESH_AUTO ? "auto" :
											cVideoDeviceInfo.latency == VIDEO_REFRESH_59_94HZ ? "59.94Hz" :
											cVideoDeviceInfo.latency == VIDEO_REFRESH_50HZ ? "50Hz" :
											cVideoDeviceInfo.latency == VIDEO_REFRESH_60HZ ? "60Hz" : "30Hz"), cVideoDeviceInfo.latency);
	Info(TAG "\tAvailable Mode Count.: %d", cVideoDeviceInfo.availableModeCount);
	Info(TAG "\tState................: %s (%d)", (cVideoDeviceInfo.state == VIDEO_STATE_DISABLED ? "disabled" :
											cVideoDeviceInfo.state == VIDEO_STATE_ENABLED ? "enabled" : "busy"), cVideoDeviceInfo.state);
	Info(TAG "\tRGB Output Range.....: %d", cVideoDeviceInfo.rgbOutputRange);
	Info(TAG "\tColor Info...........: (%d/%d,%d/%d,%d/%d,%d/%d,%d)", ci->redX, ci->redY, ci->greenX, ci->greenY, ci->blueX, ci->blueY, ci->whiteX, ci->whiteY, ci->gamma);
	Info(TAG "\tModes................: ");
	for (int i = 0; i < cVideoDeviceInfo.availableModeCount; i++)
	{
		videoDisplayMode *dm = &cVideoDeviceInfo.availableModes[i];
		Info(TAG "\t\tMode.............: %d",	i);
		Info(TAG "\t\tResolution.......: %s (%d)",	(dm->resolution == VIDEO_RESOLUTION_UNDEFINED ? "undefined" :
												dm->resolution == VIDEO_RESOLUTION_1080 ? "1080" :
												dm->resolution == VIDEO_RESOLUTION_720 ? "720" :
												dm->resolution == VIDEO_RESOLUTION_480 ? "480" :
												dm->resolution == VIDEO_RESOLUTION_576 ? "576" :
												dm->resolution == VIDEO_RESOLUTION_1600x1080 ? "1600x1080" :
												dm->resolution == VIDEO_RESOLUTION_1440x1080 ? "1440x1080" :
												dm->resolution == VIDEO_RESOLUTION_1280x1080 ? "1280x1080" :
												dm->resolution == VIDEO_RESOLUTION_960x1080 ? "960x1080" :
												dm->resolution == VIDEO_RESOLUTION_720_3D_FRAME_PACKING ? "720 3D" :
												dm->resolution == VIDEO_RESOLUTION_1024x720_3D_FRAME_PACKING ? "1024x720 3D" :
												dm->resolution == VIDEO_RESOLUTION_960x720_3D_FRAME_PACKING ? "960x720 3D" :
												dm->resolution == VIDEO_RESOLUTION_800x720_3D_FRAME_PACKING ? "800x720 3D" : "640x720 3D"), dm->resolution);
		Info(TAG "\t\tScan Mode........: %d",	dm->scanMode);
		Info(TAG "\t\tConversion.......: %d",	dm->conversion);
		Info(TAG "\t\tAspect...........: %s (%d)",	(dm->aspect == VIDEO_ASPECT_AUTO ? "auto" :
												dm->aspect == VIDEO_ASPECT_4_3 ? "4:3" : "16:9"), dm->aspect);
		Info(TAG "\t\tRefresh Rates....: %s (%d)",	(dm->refreshRates == VIDEO_REFRESH_AUTO ? "auto" :
												dm->refreshRates == VIDEO_REFRESH_59_94HZ ? "59.94Hz" :
												dm->refreshRates == VIDEO_REFRESH_50HZ ? "50Hz" :
												dm->refreshRates == VIDEO_REFRESH_60HZ ? "60Hz" : "30Hz"), dm->refreshRates);
	}

	if (videoGetState(0, 0, &cVideoState) != 0)
		return FALSE;

	if (cVideoState.state != 0)
		return FALSE;

	videoResolution res;
	if (videoGetResolution(cVideoState.displayMode.resolution, &res) != 0)
		return FALSE;

	MEMSET(&cVideoConfig, 0, sizeof(videoConfiguration));
	cVideoConfig.resolution = cVideoState.displayMode.resolution;
	cVideoConfig.format = VIDEO_BUFFER_FORMAT_XRGB;
	cVideoConfig.pitch = res.width * sizeof(u32);
	cVideoConfig.aspect = cVideoState.displayMode.aspect;

	this->WaitIdle();

	if (videoConfigure(0, &cVideoConfig, NULL, 0) != 0)
		return FALSE;

	if (videoGetState(0, 0, &cVideoState) != 0)
		return FALSE;

	gcmSetFlipMode(GCM_FLIP_VSYNC);

	iWidth = res.width;
	iHeight = res.height;

	fAspectRatio = (f32)iHeight / (f32)iWidth;

	return TRUE;
}

INLINE BOOL Screen::Initialize()
{
	Log(TAG "Initializing...");

	IScreen::SetMode(pConfiguration->GetVideoMode());
	bFading = FALSE;
	iFadeStatus = 16;

	pHostAddr = memalign(1024 * 1024, HOST_SIZE);
	pContext = rsxInit(0x100000, HOST_SIZE, pHostAddr);
	if (!pContext)
	{
		Log(TAG "ERROR: Could not create a context");
		return FALSE;
	}

	if (!this->PrepareMode())
	{
		Log(TAG "ERROR: Could not initialize video.");
		return FALSE;
	}

	for (int i = 0; i < 2; i++)
	{
		if (!this->MakeColorBuffer(&arColorBuffers[i], i))
		{
			Log(TAG "ERROR: Could not create rsx color buffer %d.", i);
			return FALSE;
		}
	}

	if (!this->MakeDepthBuffer(&sDepthBuffer))
	{
		Log(TAG "ERROR: Could not create rsx depth buffer.");
		return FALSE;
	}

	gcmResetFlipStatus();

	this->SwapSurfaces();
	iCurBuffer = 0;

	this->SetRenderTarget(pColorBuffer);

	Info(TAG "Video resolution is %dx%dx%d aspect %f.", iWidth, iHeight, iBPP, fAspectRatio);

	Log(TAG "Initialization completed.");
	return TRUE;
}

INLINE BOOL Screen::MakeColorBuffer(RSXBuffer *buf, s32 bid)
{
	int pitch = iWidth * sizeof(u32);
	int size = iHeight * pitch;

	buf->pData = (u32 *)rsxMemalign(64, size);
	if (!buf->pData)
		return FALSE;

	if (rsxAddressToOffset(buf->pData, &buf->iOffset) != 0)
	{
		rsxFree(buf->pData);
		return FALSE;
	}

	gcmSetDisplayBuffer(bid, buf->iOffset, pitch, iWidth, iHeight);
	buf->iId = bid;

	return TRUE;
}

INLINE BOOL Screen::MakeDepthBuffer(RSXBuffer *buf)
{
	int pitch = iWidth * sizeof(u32);
	int size = iHeight * pitch;

	buf->pData = (u32 *)rsxMemalign(64, size * 2);
	if (!buf->pData)
		return FALSE;

	if (rsxAddressToOffset(buf->pData, &buf->iOffset) != 0)
	{
		rsxFree(buf->pData);
		return FALSE;
	}

	return TRUE;
}

INLINE BOOL Screen::Shutdown()
{
	Log(TAG "Terminating...");

	if (pContext)
		gcmSetWaitFlip(pContext);

	iCurBuffer  = 0;
	iFadeStatus = 0;
	fadeType	= FADE_IN;
	iHeight		= PLATFORM_MAX_SCREEN_HEIGHT;
	iWidth		= PLATFORM_MAX_SCREEN_WIDTH;
	iBPP		= 32;

	if (arColorBuffers[0].pData)
		rsxFree(arColorBuffers[0].pData);
	arColorBuffers[0].pData = NULL;

	if (arColorBuffers[1].pData)
		rsxFree(arColorBuffers[1].pData);
	arColorBuffers[1].pData = NULL;

	if (sDepthBuffer.pData)
		rsxFree(sDepthBuffer.pData);
	sDepthBuffer.pData = NULL;

	if (pContext)
		rsxFinish(pContext, 0);
	pContext = NULL;

	if (pHostAddr)
		free(pHostAddr);
	pHostAddr = NULL;

	pColorBuffer = NULL;

	Log(TAG "Terminated.");

	return TRUE;
}

INLINE void Screen::WaitIdle()
{
	rsxSetWriteBackendLabel(pContext, 255, iLabel);
	rsxSetWaitLabel(pContext, 255, iLabel);
	++iLabel;

	rsxSetWriteBackendLabel(pContext, 255, iLabel);
	rsxFlushBuffer(pContext);

	while (*(vu32 *)gcmGetLabelAddress(255) != iLabel)
		usleep(30);

	++iLabel;
}

INLINE void Screen::Update()
{
	//this->SwapSurfaces();
	this->WaitFlip();
	pRendererDevice->Update();
}

INLINE void Screen::SetRenderTarget(RSXBuffer *buf)
{
	gcmSurface sf;

	sf.colorFormat		= GCM_TF_COLOR_X8R8G8B8;
	sf.colorTarget		= GCM_TF_TARGET_0;
	sf.colorLocation[0]	= GCM_LOCATION_RSX;
	sf.colorOffset[0]	= buf->iOffset;
	sf.colorPitch[0]	= iWidth * sizeof(s32);

	sf.colorLocation[1]	= GCM_LOCATION_RSX;
	sf.colorLocation[2]	= GCM_LOCATION_RSX;
	sf.colorLocation[3]	= GCM_LOCATION_RSX;
	sf.colorOffset[1]	= 0;
	sf.colorOffset[2]	= 0;
	sf.colorOffset[3]	= 0;
	sf.colorPitch[1]	= 64;
	sf.colorPitch[2]	= 64;
	sf.colorPitch[3]	= 64;

	sf.depthFormat		= GCM_TF_ZETA_Z16;
	sf.depthLocation	= GCM_LOCATION_RSX;
	sf.depthOffset		= sDepthBuffer.iOffset;
	sf.depthPitch		= iWidth * sizeof(s32);

	sf.type				= GCM_TF_TYPE_LINEAR;
	sf.antiAlias		= GCM_TF_CENTER_1;

	sf.width			= iWidth;
	sf.height			= iHeight;
	sf.x				= 0;
	sf.y				= 0;

	rsxSetSurface(pContext, &sf);
}

INLINE void Screen::FadeOut()
{
	if (bFading)
		return;

	bFading		= TRUE;
	fadeType	= FADE_OUT;
	iFadeStatus	= FADE_OUT_TRANS;
}

INLINE void Screen::FadeIn()
{
	if (bFading)
		return;

	bFading		= TRUE;
	fadeType	= FADE_IN;
	iFadeStatus	= FADE_OUT_SOLID;
}

INLINE void Screen::CancelFade()
{
	bFading		= FALSE;
	iFadeStatus	= FADE_OUT_TRANS;
}

INLINE void Screen::WaitFlip()
{
	while (gcmGetFlipStatus() != 0)
		usleep(200);

	gcmResetFlipStatus();
}

INLINE void Screen::SwapSurfaces()
{
	gcmSetFlip(pContext, arColorBuffers[iCurBuffer].iId);
	rsxFlushBuffer(pContext);
	gcmSetWaitFlip(pContext);

	iCurBuffer++;
	if (iCurBuffer >= PS3_MAX_BUFFERS)
		iCurBuffer = 0;
}

INLINE void Screen::ToggleFullscreen()
{
}

INLINE void Screen::SetMode(eVideoMode mode)
{
	UNUSED(mode);
}

INLINE BOOL Screen::HasWindowedMode() const
{
	return FALSE;
}

INLINE BOOL Screen::IsFullscreen() const
{
	return TRUE;
}

void Screen::ApplyFade()
{
	if (bFading == FALSE)
		return;

	if (fadeType == FADE_IN)
	{
		iFadeStatus -= FADE_INCREMENT;

		if (iFadeStatus <= FADE_OUT_TRANS)
		{
			bFading = FALSE;
			iFadeStatus = FADE_OUT_TRANS;
		}
	}
	else
	{
		iFadeStatus += FADE_INCREMENT;

		if (iFadeStatus >= FADE_OUT_SOLID)
		{
			bFading = FALSE;
			iFadeStatus = FADE_OUT_SOLID;
		}
	}

	u8 c = static_cast<u8>(iFadeStatus & 0xff);
	pRendererDevice->BackbufferFill(PIXEL_COLOR(0u, 0u, 0u, c));
}

}} // namespace

#endif // _PS3DEV_
