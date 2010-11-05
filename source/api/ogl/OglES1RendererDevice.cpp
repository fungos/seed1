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

/*! \file OglES1RendererDevice.cpp
	\author	Danny Angelo Carminati Grein
	\brief OpenGL ES1 renderer device implementation
*/

#include "RendererDevice.h"

#if defined(_OGL_) && defined(SEED_ENABLE_OGLES1)

#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "Texture.h"

#if defined(_IPHONE_)
#undef WideChar
#include "platform/iphone/IphoneView.h"
#include <OpenGLES/ES1/gl.h>
#endif

#define TAG "[OGLES1RendererDevice] "

namespace Seed { namespace OpenGL {

OGLES1RendererDevice::OGLES1RendererDevice()
{
	Log(TAG "Initializing...");

	char *version = (char *)glGetString(GL_VERSION);
	Info(TAG "OpenGL Version: %s", version);

	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	Info(TAG "OpenGL Maximun Texture Size: %d", maxSize);

	this->Reset();

	Log(TAG "Initialization completed.");
}

OGLES1RendererDevice::~OGLES1RendererDevice()
{
	this->Reset();
}

INLINE BOOL OGLES1RendererDevice::Initialize()
{
	BOOL ret = IRendererDevice::Initialize();

	u32 w = pScreen->GetWidth();
	u32 h = pScreen->GetHeight();
	
	if (pScreen->GetMode() == Seed::Video_iPhoneLandscape || pScreen->GetMode() == Seed::Video_iPhoneLandscapeGoofy)
	{
		w = h;
		h = pScreen->GetWidth();
	}
	
	glScissor(0, 0, w, h);
	glViewport(0, 0, w, h);

	glFrontFace(GL_CW); // defino os vertices em sentido horario
	glDisable(GL_CULL_FACE); // desabilita back face culling

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->Enable2D();

	return ret;
}

INLINE BOOL OGLES1RendererDevice::Reset()
{
	arTexture.Truncate();

	return TRUE; // abstract IRenderer::Reset();
}

INLINE BOOL OGLES1RendererDevice::Shutdown()
{
	return IRendererDevice::Shutdown();
}

INLINE void OGLES1RendererDevice::BackbufferClear(const PIXEL color)
{
	if (color)
	{
		uPixel p;
		p.pixel = color;

		glClearColor(p.rgba.r / 255.0f, p.rgba.g / 255.0f, p.rgba.b / 255.0f, p.rgba.a / 255.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
}

INLINE void OGLES1RendererDevice::Begin() const
{
#if defined(_IPHONE_)
	iphSetContext();
#endif

	this->TextureRequestProcess();
	
	glBindFramebuffer(GL_FRAMEBUFFER, pScreen->frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, pScreen->renderBuffer);

	//glClearColor(255.0f, 0.0f, 255.0f, 255.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glPushMatrix();
	glLoadIdentity();
}

INLINE void OGLES1RendererDevice::End() const
{
	glPopMatrix();
	
	pScreen->ApplyFade();
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
#if defined(_IPHONE_)
	iphContextPresentRenderBuffer();
#endif
}

BOOL OGLES1RendererDevice::CheckExtension(const char *extName)
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

INLINE void OGLES1RendererDevice::SetBlendingOperation(eBlendMode mode, PIXEL color) const
{
	switch (mode)
	{
		/* REPLACE */
		default:
		case BlendNone:
		{
			// http://home.comcast.net/~tom_forsyth/blog.wiki.html#[[Premultiplied%20alpha]]
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		break;

		/* BLEND */
		case BlendDefault:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
		break;

		case BlendMerge:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		break;

		case BlendScreen:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		}
		break;

		/* DECAL */
		case BlendDecalOverlay:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		/* MODULATE */
		case BlendAdditive:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendOverlay:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendLighten:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendColorDodge:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendModulateAlpha:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mA = PIXEL_GET_A(color);

			glColor4ub(255, 255, 255, mA);
		}
		break;

		case BlendModulate:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;
	}
}

INLINE void OGLES1RendererDevice::TextureRequestAbort(ITexture *texture)
{
	arTexture.Remove(texture);
}

INLINE void OGLES1RendererDevice::TextureRequest(ITexture *texture)
{
	arTexture.Add(texture);
}

INLINE void OGLES1RendererDevice::TextureRequestProcess() const
{
	for (u32 i = 0; i < arTexture.Size(); i++)
	{
		ITexture *texture = arTexture[i];
		if (texture)
		{
			GLint tex = 0;
			glGenTextures(1, (GLuint *)&tex);
			glBindTexture(GL_TEXTURE_2D, (GLuint)tex);

			eTextureFilter min = texture->GetFilter(Seed::TextureFilterTypeMin);
			eTextureFilter mag = texture->GetFilter(Seed::TextureFilterTypeMag);

			if (min == Seed::TextureFilterLinear)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			else if (min == Seed::TextureFilterNearest)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			if (mag == Seed::TextureFilterLinear)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			else if (mag == Seed::TextureFilterNearest)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			const void *data = texture->GetData();

			// if data == NULL then this can be a dynamic texture. we need just the texture id.
			if (data)
			{
				GLuint w = texture->GetAtlasWidthInPixel();
				GLuint h = texture->GetAtlasHeightInPixel();
				//BOOL compressed = texture->IsCompressed();
				u32 bpp = texture->GetBytesPerPixel();
				
				/*if (compressed)
				{
					GLuint bpp = 2;
					GLsizei size = w * h * bpp / 8;

					if (size < 32)
					{
						size = 32;
					}
					glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, w, h, 0, size, data);
				}
				else*/
				{
					switch (bpp)
					{
						case 4:
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
						break;

						case 3:
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
						break;

						case 2:
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
						break;

						case 1:
							glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
						break;

						default:
						break;
					}
				}
			}
			//glBindTexture(GL_TEXTURE_2D, 0);

			texture->iTextureId = tex;
			texture->Close(); // free ram
		}
	}

	arTexture.Truncate();
}

INLINE void OGLES1RendererDevice::TextureUnload(ITexture *texture)
{
	void *texId = texture->GetTextureName();
	if (texId)
	{
		GLuint *t = static_cast<GLuint *>(texId);
		GLuint tex = (GLuint)t;
		glDeleteTextures(1, &tex);
	}
}

INLINE void OGLES1RendererDevice::TextureDataUpdate(ITexture *texture)
{
	void *texId = texture->GetTextureName();
	if (texId)
	{
		GLuint *t = static_cast<GLuint *>(texId);
		GLuint tex = (GLuint)t;
		glBindTexture(GL_TEXTURE_2D, tex);

		GLuint w = texture->GetAtlasWidthInPixel();
		GLuint h = texture->GetAtlasHeightInPixel();
		const void *data = texture->GetData();
		ASSERT_NULL(data);

		switch (texture->GetBytesPerPixel())
		{
			case 4:
				// OpenGL 1.2+ only GL_EXT_bgra
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;

			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;

			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;

			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;

			default:
			break;
		}
	}
}

INLINE void OGLES1RendererDevice::UploadData(void *userData)
{
	RendererPacket *packet = static_cast<RendererPacket *>(userData);

	ITexture *texture = packet->pTexture;
	GLuint *t = static_cast<GLuint *>(texture->GetTextureName());
	GLuint tex = (GLuint)t;

	sVertex *data = static_cast<sVertex *>(packet->pVertexData);

	this->SetBlendingOperation(packet->nBlendMode, packet->iColor.pixel);

	sVertex v1 = data[0];
	sVertex v2 = data[1];
	sVertex v3 = data[2];
	sVertex v4 = data[3];

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	eTextureFilter min = texture->GetFilter(Seed::TextureFilterTypeMin);
	eTextureFilter mag = texture->GetFilter(Seed::TextureFilterTypeMag);

	if (min == Seed::TextureFilterLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (min == Seed::TextureFilterNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (mag == Seed::TextureFilterLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else if (mag == Seed::TextureFilterNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glVertexPointer(3, GL_FLOAT, sizeof(sVertex), &data[0].cVertex);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sVertex), &data[0].iColor);
	glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), &data[0].cCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, packet->iSize);
}

int OGLES1RendererDevice::GetOpenGLMeshType(eMeshType type) const
{
	UNUSED(type);
	return GL_TRIANGLE_STRIP;
}

void OGLES1RendererDevice::BackbufferFill(PIXEL color)
{
	const GLfloat vertices[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ub(PIXEL_GET_R(color), PIXEL_GET_G(color), PIXEL_GET_B(color), PIXEL_GET_A(color));
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void OGLES1RendererDevice::SetViewport(const Rect<f32> &area) const
{
	GLint x, y;
	GLsizei width, height;
	GLint w, h;
	
	w = pScreen->GetWidth();
	h = pScreen->GetHeight();
	
	if (pScreen->GetMode() == Seed::Video_iPhoneLandscape || pScreen->GetMode() == Seed::Video_iPhoneLandscapeGoofy)
	{
		w = h;
		h = pScreen->GetWidth();
	}
	
	x = static_cast<GLint>(area.x * w);
	y = static_cast<GLint>(area.y * h);
	width = static_cast<GLsizei>(area.width * w);
	height = static_cast<GLsizei>(area.height * h);

	GLint ny = h - y - height;
	glViewport(x, ny, width, height);
}

INLINE void OGLES1RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	GLfloat vertices[8];

	f32 ratio = pScreen->GetAspectRatio();
	f32 fAspectHalfWidth = w / 2.0f;
	f32 fAspectHalfHeight = (h * ratio) / 2.0f;

	// A
	vertices[0] = - fAspectHalfWidth;
	vertices[1] = - fAspectHalfHeight;

	// B
	vertices[2] = + fAspectHalfWidth;
	vertices[3] = - fAspectHalfHeight;

	if (!fill)
	{
		// C
		vertices[4] = + fAspectHalfWidth;
		vertices[5] = + fAspectHalfHeight;

		// D
		vertices[6] = - fAspectHalfWidth;
		vertices[7] = + fAspectHalfHeight;
	}
	else
	{
		// C
		vertices[4] = - fAspectHalfWidth;
		vertices[5] = + fAspectHalfHeight;

		// D
		vertices[6] = + fAspectHalfWidth;
		vertices[7] = + fAspectHalfHeight;
	}

	glColor4ub(PIXEL_GET_R(color), PIXEL_GET_G(color), PIXEL_GET_B(color), PIXEL_GET_A(color));

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	f32 nx = fAspectHalfWidth + x;
	f32 ny = fAspectHalfHeight + y * ratio;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(nx, ny, 0.0f);
		if (fill)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		else
			glDrawArrays(GL_LINE_LOOP, 0, 4);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}

// FIXME: Viewport aspect ratio...
INLINE void OGLES1RendererDevice::Enable2D() const
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Info(TAG "ERROR: Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	f32 aspect = pScreen->GetAspectRatio();
	
	// portrait
	if (pScreen->GetMode() == Seed::Video_iPhonePortrait)
	{
		glOrthof(0.0f, 1.0f, aspect - 0.001f, 0.0f, -SEED_MAX_PRIORITY, 0);
	}
	// landscape goofy
	else if (pScreen->GetMode() == Seed::Video_iPhoneLandscapeGoofy)
	{
		glOrthof(0.0f, aspect, 1.0f, 0.0f, -SEED_MAX_PRIORITY, 0);
		glTranslatef(0.0f, 1.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	}
	// landscape
	else if (pScreen->GetMode() == Seed::Video_iPhoneLandscape)
	{
		glOrthof(1.0f-aspect, 1.0f, 1.0f, 0.0f, -SEED_MAX_PRIORITY, 0);
		glTranslatef(1.0f, 0.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, 1.0f, -1.0f);

	// Save previous Renderer state
	//glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);

	glFrontFace(GL_CW);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}

INLINE void OGLES1RendererDevice::Disable2D() const
{
#if !defined(_QT_)
	// Restore previous Renderer state
	//glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
#endif
}


}} // namespace

#endif // _OGL_
