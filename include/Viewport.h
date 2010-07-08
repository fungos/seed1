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
	\brief Viewport
*/

#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "Defines.h"
#include "Rect.h"

namespace Seed {

class Renderer;

/// Viewport Interface
/**
Interface for working with viewports.
*/
class SEED_CORE_API Viewport
{
	public:
		Viewport();
		virtual ~Viewport();

		virtual void Render();

		virtual void SetRenderer(Renderer *renderer);
		virtual Renderer *GetRenderer() const;

		virtual void SetPosition(f32 x, f32 y);
		virtual void SetWidth(f32 w);
		virtual void SetHeight(f32 h);

		virtual f32 GetX() const;
		virtual f32 GetY() const;
		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;

		virtual BOOL Contains(f32 x, f32 y);

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		Rect<f32> cArea;

	private:
		SEED_DISABLE_COPY(Viewport);

	private:
		Renderer *pRenderer;
};

} // namespace


#endif // __VIEWPORT_H__
