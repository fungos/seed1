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

/*! \file IphSprite.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sprite Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Sprite.h"
#include "FileSystem.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "SeedInit.h"
#include "Formats.h"
#include "Log.h"

#include <math.h>

#include <UIKit/UIKit.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/EAGLDrawable.h>

#define TAG		"[Sprite] "

namespace Seed { namespace iPhone {

const GLubyte _no_texture_colors[] =
{
	255,   0, 255, 255,
	255,   0,   0, 255,
	0, 255,   0, 255,
	0,   0, 255, 255,
};

enum eTextureFormat
{
	kTexture2DPixelFormat_Automatic = 0,
	kTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8,
};

Sprite::Sprite()
{
	this->Reset();
}

Sprite::~Sprite()
{
	this->Reset();
}

INLINE u32 Sprite::GetWidthInPixel() const
{
	return static_cast<Texture *>(pFrame->pImage)->GetWidthInPixel();
}

INLINE u32 Sprite::GetHeightInPixel() const
{
	return static_cast<Texture *>(pFrame->pImage)->GetHeightInPixel();
}

void Sprite::Update(f32 delta)
{
	ISprite::Update(delta);

	if (!this->bChanged && !this->bTransformationChanged)
		return;

	coords[0] = fTexS0;
	coords[1] = fTexT0;
	coords[2] = fTexS1;
	coords[3] = fTexT0;
	coords[4] = fTexS0;
	coords[5] = fTexT1;
	coords[6] = fTexS1;
	coords[7] = fTexT1;

	// A
	vertices[0] = - fAspectHalfWidth;
	vertices[1] = - fAspectHalfHeight;

	// B
	vertices[2] = + fAspectHalfWidth;
	vertices[3] = - fAspectHalfHeight;

	// C
	vertices[4] = - fAspectHalfWidth;
	vertices[5] = + fAspectHalfHeight;

	// D
	vertices[6] = + fAspectHalfWidth;
	vertices[7] = + fAspectHalfHeight;

	bChanged = FALSE;
	bTransformationChanged = FALSE;
}

void Sprite::Render()
{
	Texture *image = static_cast<Texture *>(pFrame->pImage);

	GLuint texId = 0;
	if (!image)
	{
		Log(TAG "ERROR: image = NULL");
	}
	else
	{
		texId = image->LoadTexture();
	}

	if (texId)
	{
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexCoordPointer(2, GL_FLOAT, 0, coords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, _no_texture_colors);
		glEnableClientState(GL_COLOR_ARRAY);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	switch (eBlendOperation)
	{
		case IRenderable::NONE:
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		break;

		case IRenderable::BLEND:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
		break;

		case IRenderable::MERGE:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		break;

		case IRenderable::SCREEN:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		}
		break;

		/* MODULATE */
		case IRenderable::OVERLAY:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = (u8)((iVertexColor & PIXEL_R_MASK) >> PIXEL_R_SHIFT);
			u8 mG = (u8)((iVertexColor & PIXEL_G_MASK) >> PIXEL_G_SHIFT);
			u8 mB = (u8)((iVertexColor & PIXEL_B_MASK) >> PIXEL_B_SHIFT);
			u8 mA = (u8)((iVertexColor & PIXEL_A_MASK) >> PIXEL_A_SHIFT);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::LIGHTEN:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = (u8)((iVertexColor & PIXEL_R_MASK) >> PIXEL_R_SHIFT);
			u8 mG = (u8)((iVertexColor & PIXEL_G_MASK) >> PIXEL_G_SHIFT);
			u8 mB = (u8)((iVertexColor & PIXEL_B_MASK) >> PIXEL_B_SHIFT);
			u8 mA = (u8)((iVertexColor & PIXEL_A_MASK) >> PIXEL_A_SHIFT);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::COLOR_DODGE:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE, GL_ONE);

			u8 mR = (u8)((iVertexColor & PIXEL_R_MASK) >> PIXEL_R_SHIFT);
			u8 mG = (u8)((iVertexColor & PIXEL_G_MASK) >> PIXEL_G_SHIFT);
			u8 mB = (u8)((iVertexColor & PIXEL_B_MASK) >> PIXEL_B_SHIFT);
			u8 mA = (u8)((iVertexColor & PIXEL_A_MASK) >> PIXEL_A_SHIFT);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::MODULATE_ALPHA:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mA = (u8)((iVertexColor & PIXEL_A_MASK) >> PIXEL_A_SHIFT);

			glColor4ub(255, 255, 255, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::MODULATE:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = (u8)((iVertexColor & PIXEL_R_MASK) >> PIXEL_R_SHIFT);
			u8 mG = (u8)((iVertexColor & PIXEL_G_MASK) >> PIXEL_G_SHIFT);
			u8 mB = (u8)((iVertexColor & PIXEL_B_MASK) >> PIXEL_B_SHIFT);
			u8 mA = (u8)((iVertexColor & PIXEL_A_MASK) >> PIXEL_A_SHIFT);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;
	}

/*
FIXME: 2009-05-15 | CORE | GRAPHICS | WISH | Get viewport context aspect ratio | Danny Angelo Carminati Grein
*/
	f32 ratio = (static_cast<f32>(pScreen->GetHeight()) / static_cast<f32>(pScreen->GetWidth()));
	f32 x = this->fAspectHalfWidth + ISprite::GetX();
	f32 y = this->fAspectHalfHeight + ISprite::GetY() * ratio;

/*
FIXME: 08-11-2008 | IPHONE | GRAPHICS | WISH | Use glLists to optimize rendering | Danny Angelo Carminati Grein
*/
	glPushMatrix();
		glLoadIdentity();

		/* movie - check */
		glTranslatef(ISprite::GetLocalX() + x, ISprite::GetLocalY() + y, 0.0f);
		glRotatef(ISprite::GetRotation(), 0.0f, 0.0f, 1.0f);
		glScalef(ISprite::GetScaleX(), ISprite::GetScaleY(), 1.0f);
		glTranslatef(-ISprite::GetLocalX(), -ISprite::GetLocalY(), 0.0f);

		/* original */
		/*
		glTranslatef(x, y, 0.0f);
		glScalef(ISprite::GetScaleX(), ISprite::GetScaleY(), 1.0f);
		glRotatef(this->GetRotation(), 0.0f, 0.0f, 1.0f);
		*/

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	/*
	GLint cur;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &cur);
	Log(">>>>>>>>>>>>>>>>>>>>>>>>>> Image: %s Image Texture: %d  Current Texture: %d", pFrame->pImageFile, pImage->iTextureId, cur);
	*/
}

}} // namespace

#endif // _IPHONE_
