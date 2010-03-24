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

