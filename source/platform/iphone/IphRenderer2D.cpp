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

/*! \file IphRenderer2D.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer2D Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Log.h"
#include "Screen.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Sprite.h"
#include "Image.h"
#include "IphoneView.h"

#include <math.h>
#include <OpenGLES/EAGL.h>

#define TAG "[Renderer2D] "

namespace Seed { namespace iPhone {

Renderer2D::Renderer2D()
	: Renderer()
{
	Log(TAG "Initializing...");
	this->Initialize2D();

	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);

	Info(TAG "OpenGL ES Maximun Texture Size: %d", maxSize);

	Log(TAG "Initialization completed.");
}

Renderer2D::~Renderer2D()
{
}

INLINE void Renderer2D::ClearScreen(const PIXEL color) const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Pegar cor do pixel...
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepthf(1.0f);
}

INLINE void Renderer2D::Begin() const
{
	Renderer::Begin();
}

INLINE void Renderer2D::End() const
{
	Renderer::End();
}

INLINE void Renderer2D::Initialize2D()
{
	// Configura projecao ortografica

	if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		Info(TAG "ERROR: Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
	}

	glFrontFace(GL_CW); // defino os vertices em sentido horario
	glDisable(GL_CULL_FACE); // desabilita back face culling
	//glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

#if SEED_ENABLE_DEPTH_TEST
	glEnable(GL_DEPTH_TEST); // Verificar...
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_GEQUAL);
	//glClearDepthf(0.0f);
#endif // SEED_ENABLE_DEPTH_TEST
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set Viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrthof(0.5f, -0.5f, 0.5f, -0.5f, -1.0f, 1.0f);
	this->Orthogonal(static_cast<f32>(pScreen->GetWidth()), static_cast<f32>(pScreen->GetHeight()));
	//glTranslatef(-0.5f, -0.5f, 0.0f);

	Log(TAG "[WARNING] !!! Verificar se glOrtho esta correto - atualizacao do Aspect Ratio !!! [WARNING]");

	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	if (pScreen->GetMode() == Screen::LANDSCAPE)
	{
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	}
	else if (pScreen->GetMode() == Screen::LANDSCAPE_GOOFY)
	{
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	}
	else // Normal View
	{
	}
	//glTranslatef(0.5f, 0.5f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

INLINE void Renderer2D::Orthogonal(f32 width, f32 height)
{
	f32 aspectH = height / width;

	glOrthof(0.0f, 1.0f, aspectH, 0.0f, -1, 1);
}

}} // namespace

#endif // _IPHONE_
