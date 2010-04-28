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

/*! \file IphRenderer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Renderer.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "IRenderable.h"
#include "IphoneView.h"

#include <algorithm>

#define TAG "[Renderer] "

namespace Seed { namespace iPhone {

Renderer::Renderer()
	: myScreen(NULL)
	, iBufferMode(ZBufferMode)
{
	Log(TAG "Initializing...");

	this->myScreen = pScreen;

	Log(TAG "Initialization completed.");
}

Renderer::~Renderer()
{
	this->Reset();
}

void Renderer::SetBufferMode(eBufferMode mode)
{
	this->iBufferMode = mode;
}

INLINE BOOL Renderer::Reset()
{
	IRenderer::Reset();
	return TRUE;
}

void Renderer::Begin() const
{
	iphSetContext();

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, this->myScreen->frameBuffer);
	glScissor(0, 0, this->myScreen->iWidth, this->myScreen->iHeight);
	glViewport(0, 0, this->myScreen->iWidth, this->myScreen->iHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::End() const
{
	pScreen->ApplyFade();

	glBindRenderbufferOES(GL_RENDERBUFFER_OES, this->myScreen->renderBuffer);
	iphContextPresentRenderBuffer();
}

}} // namespace

#endif // _IPHONE_
