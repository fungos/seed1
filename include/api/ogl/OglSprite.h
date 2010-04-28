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

/*! \file OglSprite.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite OpenGL Implementation
*/

#ifndef __OGL_SPRITE_H__
#define __OGL_SPRITE_H__

#include "Defines.h"

#if defined(_OGL_)

#include "interface/ISprite.h"
#include "SeedInit.h"
#include "MathUtil.h"

#include <math.h>

namespace Seed { namespace OGL {

class SEED_CORE_API Sprite : public ISprite
{
	public:
		Sprite();
		virtual ~Sprite();

		// ISprite
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		// IUpdatable
		virtual void Update(f32 delta);

		// IRenderable
		virtual void Render(f32 delta);

	private:
		SEED_DISABLE_COPY(Sprite);
		virtual void SetupBlendingOperation();

	private:
		Vector3f vert[4];

#if SEED_ENABLE_DEPTH_TEST
		GLfloat vertices[12];
#else
		GLfloat vertices[8];
#endif // SEED_ENABLE_DEPTH_TEST
		GLfloat coords[8];
};

}} // namespace

#else // _OGL_
	#error "Include 'Sprite.h' instead 'api/ogl/OglSprite.h' directly."
#endif // _OGL_
#endif // __OGL_SPRITE_H__
