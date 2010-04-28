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

/*! \file WiiRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief 2D Rendering
*/

#ifndef __WII_RENDERER2D_H__
#define __WII_RENDERER2D_H__

#include "Renderer.h"
#include "interface/IRenderer2D.h"

#if defined(_WII_)

namespace Seed { namespace WII {

class Renderer2D : public IRenderer2D, public Renderer
{
	public:
		Renderer2D();
		virtual ~Renderer2D();

		virtual BOOL Reset();

		virtual void Begin() const;
		virtual BOOL Render(f32 delta);
		virtual void ClearScreen(const PIXEL color) const;

	private:
		SEED_DISABLE_COPY(Renderer2D);

		void Initialize2D();
		void PrepareDraw() const;
		void SetCamera(f32 posX, f32 posY);
		void SetViewport(u16 left, u16 top, u16 width, u16 height);

	private:
		f32 fHeight;
		f32 fWidth;
		f32 fHalfHeight;
		f32 fHalfWidth;
		f32 fCamPosX;
		f32 fCamPosY;
		f32 fCamOriX;
		f32 fCamOriY;
		u16 iLeft;
		u16 iTop;
		u16 iWidth;
		u16 iHeight;
};

}} // namespace

#else // _WII_
	#error "Include 'Renderer2D.h' instead 'platform/wii/WiiRenderer2D.h' directly."
#endif // _WII_
#endif // __WII_RENDERER2D_H__
