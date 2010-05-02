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

/*! \file OglRenderer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer OpenGL implementation
*/

#include "Renderer.h"

#if defined(_OGL_)

#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"

#if defined(__APPLE_CC__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#if defined(_SDL_)
#include "platform/sdl/SdlDefines.h"
#endif

#define TAG "[Renderer] "

// VBO
PFNGLGENBUFFERSARBPROC glGenBuffersARB = 0;
PFNGLBINDBUFFERARBPROC glBindBufferARB = 0;
PFNGLBUFFERDATAARBPROC glBufferDataARB = 0;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = 0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = 0;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = 0;
PFNGLMAPBUFFERARBPROC glMapBufferARB = 0;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = 0;

// Multi Draw Array
PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT = 0;

namespace Seed { namespace OGL {

Renderer::Renderer()
	: bufferMode()
	, bUseVBO(FALSE)
	, bHasVBO(FALSE)
{
	Log(TAG "Initializing...");

	char *version = (char *)glGetString(GL_VERSION);
	Info(TAG "OpenGL Version: %s", version);
/*
	if (this->CheckExtension("WGL_ARB_pixel_format"))
		Info(TAG "\tWGL_ARB_pixel_format");
*/
	if (this->CheckExtension("GL_EXT_bgra"))
		Info(TAG "\tGL_EXT_bgra");

	// OpenGL 1.4
	if (this->CheckExtension("GL_ARB_vertex_buffer_object"))
	{
		Info(TAG "\tGL_ARB_vertex_buffer_object");

#if defined(_SDL_)
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)SDL_GL_GetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC)SDL_GL_GetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC)SDL_GL_GetProcAddress("glBufferDataARB");
		glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)SDL_GL_GetProcAddress("glBufferSubDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)SDL_GL_GetProcAddress("glDeleteBuffersARB");
		glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)SDL_GL_GetProcAddress("glGetBufferParameterivARB");
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC)SDL_GL_GetProcAddress("glMapBufferARB");
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)SDL_GL_GetProcAddress("glUnmapBufferARB");
#else
		glGenBuffersARB = NULL;
		glBindBufferARB = NULL;
		glBufferDataARB = NULL;
		glBufferSubDataARB = NULL;
		glDeleteBuffersARB = NULL;
		glGetBufferParameterivARB = NULL;
		glMapBufferARB = NULL;
		glUnmapBufferARB = NULL;
#endif

		bUseVBO = bHasVBO = (glGenBuffersARB && glBindBufferARB && glBufferDataARB &&
							 glBufferSubDataARB && glDeleteBuffersARB && glGetBufferParameterivARB &&
							 glMapBufferARB && glUnmapBufferARB);
	}

	// OpenGL 1.4
	if (this->CheckExtension("GL_EXT_multi_draw_arrays"))
	{
		Info(TAG "\tGL_EXT_multi_draw_arrays");
#if defined(_SDL_)
		glMultiDrawArraysEXT = (PFNGLMULTIDRAWARRAYSEXTPROC)SDL_GL_GetProcAddress("glMultiDrawArraysEXT");
#else
	glMultiDrawArraysEXT = NULL;
#endif
	}

	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	Info(TAG "OpenGL Maximun Texture Size: %d", maxSize);

	this->Initialize();

	this->Reset();
	Log(TAG "Initialization completed.");
}

Renderer::~Renderer()
{
	this->Reset();
}

INLINE BOOL Renderer::Initialize()
{
#if !defined(_QT_)
	IRenderer::Initialize();

	glScissor(0, 0, pScreen->GetWidth(), pScreen->GetHeight());
	glViewport(0, 0, pScreen->GetWidth(), pScreen->GetHeight());
#endif
	return TRUE;
}

INLINE BOOL Renderer::Reset()
{
	return TRUE; // abstract IRenderer::Reset();
}

INLINE void Renderer::SetBufferMode(eBufferMode mode)
{
	this->bufferMode = mode;
}

INLINE void Renderer::Begin() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

INLINE void Renderer::End() const
{
	pScreen->ApplyFade();
}

INLINE void Renderer::BeginRenderMask() const
{
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

INLINE void Renderer::BeginRenderMasked() const
{
	glDisable(GL_ALPHA_TEST);

	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

INLINE void Renderer::BeginRenderUnmasked() const
{
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

INLINE void Renderer::BeginRenderSpecial() const
{
	glDisable(GL_STENCIL_TEST);
}

BOOL Renderer::CheckExtension(const char *extName)
{
	/*
	 ** Search for extName in the extensions string.  Use of strstr()
	 ** is not sufficient because extension names can be prefixes of
	 ** other extension names.  Could use strtok() but the constant
	 ** string returned by glGetString can be in read-only memory.
	 */
	char *p = (char *)glGetString(GL_EXTENSIONS);
	char *end;
	size_t extNameLen;

	extNameLen = strlen(extName);
	end = p + strlen(p);

	while (p < end)
	{
		size_t n = strcspn(p, " ");
		if ((extNameLen == n) && (strncmp(extName, p, n) == 0))
		{
			return TRUE;
		}

		p += (n + 1);
	}

	return FALSE;
}

INLINE GLint Renderer::GetOpenGLMeshType(eMeshType type)
{
	GLint ret = GL_TRIANGLE_STRIP;

	switch (type)
	{
		case Seed::Triangles:
		{
			ret = GL_TRIANGLES;
		}
		break;

		case Seed::LineStrip:
		{
			ret = GL_LINE_STRIP;
		}
		break;

		case Seed::TriangleStrip:
		default:
		{
		}
		break;
	}

	return ret;
}

}} // namespace

#endif // _OGL_
