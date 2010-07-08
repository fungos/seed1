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

/*! \file IRenderable.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderable object interface
*/

#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

#include "Defines.h"
#include "Enum.h"
#include "interface/IObject.h"

namespace Seed {

class IRenderer;
class ITexture;

/// Drawable Interface
/**
Interface for renderables objects. Things that will be rendered to the screen.
*/
class SEED_CORE_API IRenderable
{
	public:
		IRenderable();
		virtual ~IRenderable();

		virtual void Render();
		virtual void Update(f32 delta);
		virtual void SetColor(u32 r, u32 g, u32 b, u32 a);
		virtual void SetColor(f32 r, f32 g, f32 b, f32 a);
		virtual void SetColor(PIXEL px);
		virtual PIXEL GetColor() const;

		virtual void SetBlending(eBlendMode op);
		virtual void SetVisible(BOOL b);
		BOOL IsVisible() const;

	protected:
		eBlendMode	eBlendOperation;
		uPixel		iColor;
		BOOL		bVisible;

		virtual void Reset();

	private:
		SEED_DISABLE_COPY(IRenderable);
};

} // namespace

#endif // __IRENDERABLE_H__
