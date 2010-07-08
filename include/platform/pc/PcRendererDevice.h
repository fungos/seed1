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

/*! \file PcRendererDevice.h
	\author	Danny Angelo Carminati Grein
	\brief Renderer device factory
*/

#ifndef __PC_RENDERER_DEVICE_H__
#define __PC_RENDERER_DEVICE_H__

#include "Defines.h"

#if defined(_PC_)

#include "Singleton.h"
#include "interface/IRendererDevice.h"

namespace Seed { namespace PC {

class RendererDevice : public IRendererDevice
{
	SEED_SINGLETON_DECLARE(RendererDevice);
	public:
		// IRendererDevice
		virtual void TextureUnload(ITexture *tex);
		virtual void TextureRequest(ITexture *texture, void **texName);
		virtual void TextureRequestAbort(ITexture *texture, void **texName);
		virtual void TextureRequestProcess() const;
		virtual void TextureDataUpdate(ITexture *texture);

		virtual void SetBlendingOperation(eBlendMode mode, PIXEL color) const;
		virtual void UploadData(void *userData);
		virtual void BackbufferClear(const PIXEL color = 0);
		virtual void BackbufferFill(const PIXEL color = 0);

		virtual void Begin() const;
		virtual void End() const;

		virtual void SetViewport(const Rect<f32> &area) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill = FALSE) const;
		virtual void Enable2D() const;
		virtual void Disable2D() const;

		virtual void Update();

		// IModule
		virtual BOOL Initialize();
		virtual BOOL Reset();
		virtual BOOL Shutdown();

	private:
		SEED_DISABLE_COPY(RendererDevice);

		IRendererDevice cNull;
		IRendererDevice	*pApiDevice;
};

#define pRendererDevice RendererDevice::GetInstance()

}} // namespace

#endif // _PC_

#endif // __PC_RENDERER_DEVICE_H__
