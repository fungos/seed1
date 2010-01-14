/*! \file OglSprite.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sprite OpenGL Implementation
*/


#include "Sprite.h"

#ifdef _OGL_

#include "FileSystem.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "SeedInit.h"
#include "Formats.h"
#include "Log.h"
#include "Image.h"
#include "SpriteObject.h"
#include "Renderer2D.h"
#include "Enum.h"

#include <math.h>
#include <GL/glext.h>

#define TAG		"[Sprite] "

#if DEBUG_ENABLE_RECT_SPRITE == 1
#define DEBUG_SPRITE_RECT DEBUG_RECT(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), DEBUG_RECT_COLOR_SPRITE);
#else
#define DEBUG_SPRITE_RECT
#endif


namespace Seed { namespace OGL {


const GLubyte _no_texture_colors[] =
{
	255, 0, 255, 255,
	255, 0,   0, 255,
	0, 255,   0, 255,
	0,   0, 255, 255,
};


Sprite::Sprite()
{
	arCurrentVertexData = &vert[0];
	this->Reset();
}

Sprite::~Sprite()
{
	this->Reset();
}

u32 Sprite::GetWidthInPixel() const
{
	//return static_cast<Image *>(pFrame->pImage)->GetWidthInPixel();
	return pFrameImage->GetWidthInPixel();
}

u32 Sprite::GetHeightInPixel() const
{
	//return static_cast<Image *>(pFrame->pImage)->GetHeightInPixel();
	return pFrameImage->GetHeightInPixel();
}

void Sprite::Update(f32 delta)
{
	ISprite::Update(delta);

	if (!bChanged && !IsChanged()) //!bTransformationChanged)
		return;

	if (!arCustomCoordsData)
	{
		coords[0] = fTexS0;
		coords[1] = fTexT0;
		coords[2] = fTexS1;
		coords[3] = fTexT0;
		coords[4] = fTexS0;
		coords[5] = fTexT1;
		coords[6] = fTexS1;
		coords[7] = fTexT1;
	}

	if (!arCustomVertexData)
	{
		#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
			arCurrentVertexData = &vert[0];
			vert[0] = Vector3f(-(f32)iHalfWidth, -(f32)iHalfHeight, fPriority);
			vert[1] = Vector3f(+(f32)iHalfWidth, -(f32)iHalfHeight, fPriority);
			vert[2] = Vector3f(-(f32)iHalfWidth, +(f32)iHalfHeight, fPriority);
			vert[3] = Vector3f(+(f32)iHalfWidth, +(f32)iHalfHeight, fPriority);
		#else
			arCurrentVertexData = &vert[0];
			vert[0] = Vector3f(-fAspectHalfWidth, -fAspectHalfHeight, fPriority);
			vert[1] = Vector3f(+fAspectHalfWidth, -fAspectHalfHeight, fPriority);
			vert[2] = Vector3f(-fAspectHalfWidth, +fAspectHalfHeight, fPriority);
			vert[3] = Vector3f(+fAspectHalfWidth, +fAspectHalfHeight, fPriority);
		#endif
	}
	else
	{
		arCurrentVertexData = arCustomVertexData;
	}

	/* Mover para viewport */
	f32 fScreenWidth	= (f32)pScreen->GetWidth();
	f32 fScreenHeight	= (f32)pScreen->GetHeight();
	/* ^^^^ */

	f32 x, y;
	#ifdef SEED_USE_REAL_COORDINATE_SYSTEM
		x = this->iHalfWidth + ISprite::GetX();
		y = this->iHalfHeight + ISprite::GetY();
	#else
		f32 ratio			= fScreenHeight / fScreenWidth;
		x = this->fAspectHalfWidth + ISprite::GetX();
		y = this->fAspectHalfHeight + ISprite::GetY() * ratio;
	#endif

	f32 lx = ISprite::GetLocalX();
	f32 ly = ISprite::GetLocalY();

	Matrix4x4f t1, t2, r, s;
	t1 = TranslationMatrix(lx + x, ly + y, 0.0f);
	r = RotationMatrix(AxisZ, DegToRad(ISprite::GetRotation()));
	s = ScaleMatrix(ISprite::GetScaleX(), ISprite::GetScaleY(), 1.0f);
	t2 = TranslationMatrix(-lx, -ly, 0.0f);
	Matrix4x4f transform = ((t1 * r) * s) * t2;

	if (bTransformationChanged || !arCustomVertexData)
	{
		for (u32 i = 0; i < iNumVertices; i++)
		{
			transform.Transform(arCurrentVertexData[i]);
		}
	}

	bChanged = FALSE;
	bTransformationChanged = FALSE;
}

void Sprite::Render(f32 delta)
{
	this->Update(delta);
	UNUSED(delta);

	Image *image = static_cast<Image *>(pFrameImage);
	ASSERT(image);

	this->SetupBlendingOperation();

#if 1 // Vertex Array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// TOP 146 FPS
	// AVG 144 FPS
	glBindTexture(GL_TEXTURE_2D, image->LoadTexture());
	if (arCustomCoordsData)
		glTexCoordPointer(2, GL_FLOAT, 0, arCustomCoordsData);
	else
		glTexCoordPointer(2, GL_FLOAT, 0, coords);

	//glVertexPointer(3, GL_FLOAT, 0, vert);
	glVertexPointer(3, GL_FLOAT, 0, arCurrentVertexData);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
		glLoadIdentity();
		glDrawArrays(Renderer::GetOpenGLMeshType(nMeshType), 0, iNumVertices);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#elif 0 // Immediate mode A
	// TOP 160 FPS
	// AVG 159 FPS
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(coords[0], coords[1]);
		glVertex3f(vert[0].x, vert[0].y, vert[0].z);

		glTexCoord2f(coords[2], coords[3]);
		glVertex3f(vert[1].x, vert[1].y, vert[1].z);

		glTexCoord2f(coords[4], coords[5]);
		glVertex3f(vert[2].x, vert[2].y, vert[2].z);

		glTexCoord2f(coords[6], coords[7]);
		glVertex3f(vert[3].x, vert[3].y, vert[3].z);
	glEnd();
	glPopMatrix();
#elif 0 // Immediate mode B
	RendererPacket packet;
	packet.iSize = 4;
	packet.iType = PACKET_TYPE_DISPLAYLIST;
	packet.pTexCoordData = coords;
	packet.pVertexData = vert;

	Renderer2D *rend = static_cast<Renderer2D *>(Private::pRenderer);
	ASSERT(rend);
	rend->SelectTexture(image->LoadTexture());
/*
	rend->UploadData(&packet);
*/

	Vector3f *vertex = static_cast<Vector3f *>(packet.pVertexData);
	struct tc { GLfloat t[2]; };
	tc *texcoord = static_cast<tc *>(packet.pTexCoordData);

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		for (u32 i = 0; i < packet.iSize; i++)
		{
			// TOP 159 FPS
			// AVG 158 FPS
			glTexCoord2f(texcoord[i].t[0], texcoord[i].t[1]);
			glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);

			// TOP 158 FPS
			// AVG 157 FPS
			//glTexCoord2fv(texcoord[i].t);
			//glVertex3fv(vertex[i]);
		}
	glEnd();
	glPopMatrix();
#else
	// TOP 154 FPS
	// AVG 152 FPS
	if (iNumVertices > 4)
		int a =0;

	RendererPacket packet;
	packet.iSize = iNumVertices;
	packet.nMeshType = Renderer::GetOpenGLMeshType(nMeshType);
	packet.pTexCoordData = arCustomCoordsData ? arCustomCoordsData : coords;
	packet.pVertexData = arCurrentVertexData;

	Renderer2D *rend = static_cast<Renderer2D *>(Private::pRenderer);
	ASSERT(rend);

	rend->SetPacketType(PACKET_TYPE_VBO);
	//rend->SetPacketType(PACKET_TYPE_IMMEDIATE);
	//rend->SetPacketType(PACKET_TYPE_DRAWARRAY);
	//rend->SetPacketType(PACKET_TYPE_DISPLAYLIST);
	rend->SelectTexture(image->LoadTexture());
	rend->UploadData(&packet);
#endif

#if SEED_ENABLE_PRELOAD_TEXTURE == 0
	image->UnloadTexture();
#endif // SEED_ENABLE_PRELOAD_TEXTURE

	//DEBUG_SPRITE_RECT;
}

void Sprite::SetupBlendingOperation()
{
	switch (eBlendOperation)
	{
		/* REPLACE */
		default:
		case IRenderable::NONE:
		{
			// http://home.comcast.net/~tom_forsyth/blog.wiki.html#[[Premultiplied%20alpha]]
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}
		break;

		/* BLEND */
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
			// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			// glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE); // Morpho
		}
		break;

		/* DECAL */
		case IRenderable::DECAL_OVERLAY:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(iColor);
			u8 mG = PIXEL_GET_G(iColor);
			u8 mB = PIXEL_GET_B(iColor);
			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		/* MODULATE */
		case IRenderable::ADDITIVE:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			u8 mR = PIXEL_GET_R(iColor);
			u8 mG = PIXEL_GET_G(iColor);
			u8 mB = PIXEL_GET_B(iColor);
			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::OVERLAY:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(iColor);
			u8 mG = PIXEL_GET_G(iColor);
			u8 mB = PIXEL_GET_B(iColor);
			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::LIGHTEN:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(iColor);
			u8 mG = PIXEL_GET_G(iColor);
			u8 mB = PIXEL_GET_B(iColor);
			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::COLOR_DODGE:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE, GL_ONE);

			u8 mR = PIXEL_GET_R(iColor);
			u8 mG = PIXEL_GET_G(iColor);
			u8 mB = PIXEL_GET_B(iColor);
			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::MODULATE_ALPHA:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(255, 255, 255, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;

		case IRenderable::MODULATE:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(iColor);
			u8 mG = PIXEL_GET_G(iColor);
			u8 mB = PIXEL_GET_B(iColor);
			u8 mA = PIXEL_GET_A(iColor);

			glColor4ub(mR, mG, mB, mA);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		break;
	}
}


}} // namespace


#endif // _OGL_
