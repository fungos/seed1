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

/*! \file OglRenderer.h
	\author	Danny Angelo Carminati Grein
	\brief Renderer OpenGL implementation
*/


#ifndef __OGL_RENDERER_H__
#define __OGL_RENDERER_H__

#include "Defines.h"
#include "Enum.h"

#ifdef _OGL_

#include "interface/IRenderer.h"

#if defined(__APPLE_CC__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

// VBO
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;

// Multi Draw Array
extern PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT;


namespace Seed { namespace OGL {


class Renderer : public IRenderer
{
	friend class IScreen;

	public:
		Renderer();
		virtual ~Renderer();

		virtual void SetBufferMode(eBufferMode mode);
		virtual BOOL Reset();
		virtual void Begin() const;
		virtual void End() const;
		virtual void BeginRenderMask() const;
		virtual void BeginRenderMasked() const;
		virtual void BeginRenderUnmasked() const;
		virtual void BeginRenderSpecial() const;
		static GLint GetOpenGLMeshType(eMeshType type);

	protected:
		BOOL Initialize();

	protected:
		eBufferMode bufferMode;
		BOOL bUseVBO;
		BOOL bHasVBO;

	private:
		SEED_DISABLE_COPY(Renderer);

		BOOL CheckExtension(const char *extName);
};


}} // namespace


#else // _OGL_

	#error "Include 'Renderer.h' instead 'api/ogl/OglRenderer.h' directly."

#endif // _OGL__
#endif // __OGL_RENDERER_H__

