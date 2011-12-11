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

/*! \file PS3RendererDevice.cpp
	\author	Danny Angelo Carminati Grein
	\brief RSX ps3dev renderer device implementation
*/

#include "RendererDevice.h"

#if defined(_PS3DEV_)

#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "Texture.h"

#define TAG "[RSXRendererDevice] "

namespace Seed { namespace PS3 {

SEED_SINGLETON_DEFINE(RSXRendererDevice)

INLINE BOOL RSXRendererDevice::Initialize()
{
	BOOL ret = IRendererDevice::Initialize();

	u32 w = pScreen->GetWidth();
	u32 h = pScreen->GetHeight();

	pContext = pScreen->pContext;

	rsxSetColorMask(pContext, GCM_COLOR_MASK_B | GCM_COLOR_MASK_G | GCM_COLOR_MASK_R | GCM_COLOR_MASK_A);
	rsxSetColorMaskMRT(pContext, 0); // Disable access to the Frame Buffer (Multi Render Target)

	f32 scale[4];
	f32 offset[4];
	f32 min = 0.0f;
	f32 max = 1.0f;
	f32 x = 0.0f;
	f32 y = 0.0f;

	scale[0] = w * 0.5f;
	scale[1] = h * -0.5f;
	scale[2] = (max - min )* 0.5f;
	scale[3] = 0.0f;

	offset[0] = x + w * 0.5f;
	offset[1] = y + h * 0.5f;
	offset[2] = (max + min) * 0.5f;
	offset[3] = 0.0f;

	rsxSetScissor(pContext, x, y, w, h);
	rsxSetViewport(pContext, x, y, w, h, min, max, scale, offset);

	rsxSetFrontFace(pContext, GCM_FRONTFACE_CW); // defino os vertices em sentido horario
	rsxSetCullFaceEnable(pContext, GCM_FALSE); // desabilita back face culling

	//glEnable(GL_TEXTURE_2D);
	rsxSetBlendEnable(pContext, GCM_TRUE);

//	rsxSetDepthTestEnable(pContext, GCM_TRUE);
//	rsxSetDepthFunc(pContext, GCM_LESS);
//	rsxSetShadeModel(pContext, GCM_SHADE_MODEL_SMOOTH);
//	rsxSetDepthWriteEnable(pContext, 1);

	rsxSetClearColor(pContext, PIXEL_COLOR(0u, 0u, 0u, 1u));

	//glClearStencil(0);
	rsxSetClearDepthValue(pContext, 0);
	rsxClearSurface(pContext, GCM_CLEAR_M);

	this->Enable2D();

	return ret;
}

INLINE BOOL RSXRendererDevice::Reset()
{
	arTexture.Truncate();

	return TRUE; // abstract IRenderer::Reset();
}

INLINE BOOL RSXRendererDevice::Shutdown()
{
	return IRendererDevice::Shutdown();
}

INLINE void RSXRendererDevice::BackbufferClear(const PIXEL color)
{
	if (color)
	{
		uPixel p;
		p.pixel = color;

		rsxSetClearColor(pContext, color);
	}

	rsxClearSurface(pContext, GCM_CLEAR_R | GCM_CLEAR_G | GCM_CLEAR_B | GCM_CLEAR_A);
	rsxSetClearColor(pContext, PIXEL_COLOR(0u, 0u, 0u, 1u));
}

INLINE void RSXRendererDevice::Begin() const
{
	this->TextureRequestProcess();

//	glPushMatrix();
//	glLoadIdentity();
}

INLINE void RSXRendererDevice::End() const
{
//	glPopMatrix();
	pScreen->ApplyFade();
	pScreen->SwapSurfaces();
}

INLINE void RSXRendererDevice::SetBlendingOperation(eBlendMode mode, PIXEL color) const
{
	switch (mode)
	{
		/* REPLACE */
		default:
		case BlendNone:
		{
			// http://home.comcast.net/~tom_forsyth/blog.wiki.html#[[Premultiplied%20alpha]]
//			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		break;

		/* BLEND */
		case BlendDefault:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
//			glBlendFunc(GL_ONE, GL_ONE);
		}
		break;

		case BlendMerge:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		break;

		case BlendScreen:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
//			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		}
		break;

		/* DECAL */
		case BlendDecalOverlay:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(mR, mG, mB, mA);
		}
		break;

		/* MODULATE */
		case BlendAdditive:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendOverlay:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendLighten:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendColorDodge:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glBlendFunc(GL_ONE, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendModulateAlpha:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(255, 255, 255, mA);
		}
		break;

		case BlendModulate:
		{
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

//			glColor4ub(mR, mG, mB, mA);
		}
		break;
	}
}

INLINE void RSXRendererDevice::TextureRequestAbort(ITexture *texture)
{
	arTexture.Remove(texture);
}

INLINE void RSXRendererDevice::TextureRequest(ITexture *texture)
{
	arTexture.Add(texture);
}

INLINE void RSXRendererDevice::TextureRequestProcess() const
{
	for (u32 i = 0; i < arTexture.Size(); i++)
	{
		ITexture *texture = arTexture[i];
		if (texture)
		{
/*
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

			GLuint w = texture->GetAtlasWidthInPixel();
			GLuint h = texture->GetAtlasHeightInPixel();
			const void *data = texture->GetData();

			// if data == NULL then this can be a dynamic texture. we need just the texture id.
			if (data)
			{
				switch (texture->GetBytesPerPixel())
				{
					case 4:
						// OpenGL 1.2+ only GL_EXT_bgra
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
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
			//glBindTexture(GL_TEXTURE_2D, 0);

			texture->iTextureId = tex;
			texture->Close(); // free ram
*/
		}
	}

	arTexture.Truncate();
}

INLINE void RSXRendererDevice::TextureUnload(ITexture *texture)
{
//	if (texture->iTextureId)
//		glDeleteTextures(1, &texture->iTextureId);
}

INLINE void RSXRendererDevice::TextureDataUpdate(ITexture *texture)
{
	if (texture->iTextureId)
	{
/*
		glBindTexture(GL_TEXTURE_2D, texture->iTextureId);

		GLuint w = texture->GetAtlasWidthInPixel();
		GLuint h = texture->GetAtlasHeightInPixel();
		const void *data = texture->GetData();
		ASSERT_NULL(data);

		switch (texture->GetBytesPerPixel())
		{
			case 4:
				// OpenGL 1.2+ only GL_EXT_bgra
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
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
*/
	}
}

INLINE void RSXRendererDevice::UploadData(void *userData)
{
	RendererPacket *packet = static_cast<RendererPacket *>(userData);

	ITexture *texture = packet->pTexture;
	sVertex *data = static_cast<sVertex *>(packet->pVertexData);

//	glPushMatrix();
//	glLoadIdentity();

	this->SetBlendingOperation(packet->nBlendMode, packet->iColor.pixel);
/*
	glBindTexture(GL_TEXTURE_2D, texture->iTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/
	eTextureFilter min = texture->GetFilter(Seed::TextureFilterTypeMin);
	eTextureFilter mag = texture->GetFilter(Seed::TextureFilterTypeMag);
/*
	if (min == Seed::TextureFilterLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (min == Seed::TextureFilterNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (mag == Seed::TextureFilterLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else if (mag == Seed::TextureFilterNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(this->GetOpenGLMeshType(packet->nMeshType));
	for (u32 i = 0; i < packet->iSize; i++)
	{
		glTexCoord2f(data[i].cCoords.x, data[i].cCoords.y);
		glVertex3f(data[i].cVertex.x, data[i].cVertex.y, data[i].cVertex.z);
	}
	glEnd();

	glPopMatrix();
*/
}

int RSXRendererDevice::GetRSXMeshType(eMeshType type) const
{
/*
#define	GCM_TYPE_POINTS				1
#define	GCM_TYPE_LINES				2
#define	GCM_TYPE_LINE_LOOP			3
#define	GCM_TYPE_LINE_STRIP			4
#define	GCM_TYPE_TRIANGLES			5
#define	GCM_TYPE_TRIANGLE_STRIP		6
#define	GCM_TYPE_TRIANGLE_FAN		7
#define	GCM_TYPE_QUADS				8
#define	GCM_TYPE_QUAD_STRIP			9
#define	GCM_TYPE_POLYGON			10
*/
	int val = GCM_TYPE_TRIANGLE_STRIP;

	switch (type)
	{
		case Triangles:
		{
			val = GCM_TYPE_TRIANGLES;
		}
		break;

		case LineStrip:
		{
			val = GCM_TYPE_LINE_STRIP;
		}
		break;

		case Quads:
		{
			val = GCM_TYPE_QUADS;
		}
		break;

		case TriangleStrip:
		default:
		{
		}
		break;
	}

	return v;
}

void RSXRendererDevice::BackbufferFill(PIXEL color)
{
	rsxSetBlendEnable(pContext, GCM_TRUE);
	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // ?? frament shader?
	rsxSetBlendFunc(pContext, GCM_SRC_ALPHA, GCM_ONE_MINUS_SRC_ALPHA, GCM_SRC_ALPHA, GCM_ONE_MINUS_SRC_ALPHA);
	//glColor4ub(PIXEL_GET_R(color), PIXEL_GET_G(color), PIXEL_GET_B(color), PIXEL_GET_A(color));

	rsxDrawVertexBegin(pContext, GCM_TYPE_TRIANGLE_STRIP);
		rsxDrawVertex2f(pContext, 0, 0.0f, 0.0f);
		rsxDrawVertex2f(pContext, 1, 0.0f, 1.0f);
		rsxDrawVertex2f(pContext, 2, 1.0f, 0.0f);
		rsxDrawVertex2f(pContext, 3, 1.0f, 1.0f);
	rsxDrawVertexEnd(pContext);
}

void RSXRendererDevice::SetViewport(const Rect<f32> &area) const
{
	f32 x = static_cast<f32>(area.x * pScreen->GetWidth());
	f32 y = static_cast<f32>(area.y * pScreen->GetHeight());
	f32 w = static_cast<f32>(area.width * pScreen->GetWidth());
	f32 h = static_cast<f32>(area.height * pScreen->GetHeight());

	f32 scale[4];
	f32 offset[4];
	f32 min = 0.0f;
	f32 max = 1.0f;

	scale[0] = w * 0.5f;
	scale[1] = h * -0.5f;
	scale[2] = (max - min )* 0.5f;
	scale[3] = 0.0f;

	// FIXME: Do the correct math... and test afterwards
	offset[0] = x + (w * 0.5f);
	offset[1] = y + (h * 0.5f);
	offset[2] = (max + min) * 0.5f;
	offset[3] = 0.0f;

	rsxSetViewport(pContext, x, pScreen->GetHeight() - y - h, w, h, min, max, scale, offset);
}

INLINE void RSXRendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
/*
	GLfloat vertices[8];

	f32 ratio = ((f32)pScreen->GetHeight() / (f32)pScreen->GetWidth());
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
*/
}

// FIXME: Viewport aspect ratio...
INLINE void RSXRendererDevice::Enable2D() const
{
	f32 viewW = static_cast<f32>(pScreen->GetWidth());
	f32 viewH = static_cast<f32>(pScreen->GetHeight());
/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	f32 aspectH = viewH / viewW;
	glOrtho(0.0f, 1.0f, aspectH, 0.0f, -SEED_MAX_PRIORITY, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, 1.0f, -1.0f);

	// Save previous Renderer state
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
*/
}

INLINE void RSXRendererDevice::Disable2D() const
{
/*
	// Restore previous Renderer state
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
*/
}


}} // namespace

#endif // _PS3DEV_
