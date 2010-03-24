/*! \file WiiRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief 2D Rendering
*/

#ifndef __WII_RENDERER2D_H__
#define __WII_RENDERER2D_H__

#include "Renderer.h"
#include "interface/IRenderer2D.h"

#ifdef _WII_

namespace Seed { namespace WII {

class Renderer2D : public IRenderer2D, public Renderer
{
	public:
		Renderer2D();
		virtual ~Renderer2D();

		virtual BOOL Reset();

		virtual void Begin() const;
		virtual BOOL Render(f32 delta);
		virtual void ClearScreen(const PIXEL color) const;

	private:
		SEED_DISABLE_COPY(Renderer2D);

		void Initialize2D();
		void PrepareDraw() const;
		void SetCamera(f32 posX, f32 posY);
		void SetViewport(u16 left, u16 top, u16 width, u16 height);

	private:
		f32 fHeight;
		f32 fWidth;
		f32 fHalfHeight;
		f32 fHalfWidth;
		f32 fCamPosX;
		f32 fCamPosY;
		f32 fCamOriX;
		f32 fCamOriY;
		u16 iLeft;
		u16 iTop;
		u16 iWidth;
		u16 iHeight;
};

}} // namespace

#else // _WII_
	#error "Include 'Renderer2D.h' instead 'platform/wii/WiiRenderer2D.h' directly."
#endif // _WII_
#endif // __WII_RENDERER2D_H__
