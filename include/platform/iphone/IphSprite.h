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

/*! \file IphSprite.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite Iphone Implementation
*/

#ifndef __IPH_SPRITE_H__
#define __IPH_SPRITE_H__

#if defined(_IPHONE_)

#include "Defines.h"
#include "interface/ITexture.h"
#include "interface/ISprite.h"
#include "SeedInit.h"
#include "SpriteObject.h"
#include "Texture.h"
#include "ResourceManager.h"

#include <math.h>

namespace Seed { namespace iPhone {

class Sprite : public ISprite
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

		virtual void ReconfigureFrame();

	private:
		const void	*pData;

		GLfloat vertices[8];
		GLfloat coords[8];
};

}} // namespace

#else // _IPHONE_
	#error "Include 'Sprite.h' instead 'platform/iphone/IphSprite.h' directly."
#endif // _IPHONE_
#endif // __IPH_SPRITE_H__
