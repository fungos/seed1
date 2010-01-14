/*! \file IphRenderer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer Iphone Implementation
*/

#ifdef _IPHONE_

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

