/*! \file OglRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief 2D Renderer OpenGL implementation
*/


#ifndef __OGL_RENDERER2D_H__
#define __OGL_RENDERER2D_H__

#include "Defines.h"

#if defined(_OGL_)

#include "Renderer.h"
#include "interface/IRenderer2D.h"
#include "Matrix4x4.h"

#define SEED_RENDERER_DATA_MAX		(1024 * 100)
#define SEED_RENDERER_PACKET_MAX	(1024 * 2)

#define PACKET_TYPE_IMMEDIATE		0x01
#define PACKET_TYPE_DRAWARRAY		0x02
#define PACKET_TYPE_DISPLAYLIST		0x03

/*
VBO is better when usign for one large object. It can be used with a lot of tiny objects,
but they must have the same setings (eg. texture and blending). Even thought vertex,
texcoord and normal can differ.
*/ 
#define PACKET_TYPE_VBO				0x04


namespace Seed { namespace OGL {


struct RendererPacket
{
	GLint	nMeshType;
	void	*pVertexData;
	void	*pTexCoordData;
	u32		iSize;
};

class Renderer2D : public IRenderer2D, public Renderer
{
	public:
		Renderer2D();
		virtual ~Renderer2D();

		virtual void SelectTexture(u32 texId);
		virtual void SetPacketType(u32 type);
		virtual void UploadData(void *userData);
		virtual void CommitData() const;

		virtual BOOL Initialize();
		virtual BOOL Shutdown();

		virtual void Begin() const;
		virtual void End() const;

		virtual void ClearScreen(const PIXEL color = 0) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill = FALSE) const;

	private:
		SEED_DISABLE_COPY(Renderer2D);

		void Enable2D();
		void Disable2D();

		void Begin(u32 type) const;
		void End(u32 type) const;

		void UseVertexBuffer(RendererPacket *packet);
		void UseDisplayList(RendererPacket *packet);
		void UseImmediate(RendererPacket *packet);
		void UseDrawArray(RendererPacket *packet);

	private:
		f32			fScreenW;
		f32			fScreenH;

		mutable BOOL bInsideDisplayList;

		u32			iTextureId;
		mutable u32	iDataCount;
		mutable u32	iPacketCount;
		u32			iLastType;

		GLuint		iVertexVboId;
		GLuint		iTexCoordVboId;
		GLuint		iDisplayList;
		GLint		nVboMeshType;

		mutable Vector3f	arVertexData[SEED_RENDERER_DATA_MAX];
		mutable f32			arTexCoordData[SEED_RENDERER_DATA_MAX * 2];

		mutable GLint		arPacketIndex[SEED_RENDERER_PACKET_MAX];
		mutable GLsizei		arPacketCount[SEED_RENDERER_PACKET_MAX];
};


}} // namespace


#else // _OGL_

	#error "Include 'Renderer2D.h' instead 'api/ogl/OglRenderer2D.h' directly."

#endif // _OGL_
#endif // __OGL_RENDERER2D_H__
