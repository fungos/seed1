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

/*! \file IphRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief Renderer2D Iphone Implementation
*/

#ifndef __IPH_RENDERER2D_H__
#define __IPH_RENDERER2D_H__

#include "Renderer.h"
#include "interface/IRenderer2D.h"

#if defined(_IPHONE_)

namespace Seed { namespace iPhone {

class Renderer2D : public Renderer, public Seed::IRenderer2D
{
	public:
		Renderer2D();
		virtual ~Renderer2D();

		virtual void Begin() const;
		virtual void End() const;
		virtual void ClearScreen(const PIXEL color) const;

	private:
		SEED_DISABLE_COPY(Renderer2D);

		void Initialize2D();
		void SetCamera(f32 posX, f32 posY);
		void SetViewport(u16 left, u16 top, u16 width, u16 height);
		void Orthogonal(f32 width, f32 height);
};

}} // namespace

#else // _IPHONE_
	#error "Include 'Renderer2D.h' instead 'platform/iphone/IphRenderer2D.h' directly."
#endif // _IPHONE_
#endif // __IPH_RENDERER2D_H__
