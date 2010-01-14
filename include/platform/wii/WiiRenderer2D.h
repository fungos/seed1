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
	private:
		f32 fHeight, fWidth, fHalfHeight, fHalfWidth;
		f32 fCamPosX, fCamPosY, fCamOriX, fCamOriY;
		u16 iLeft, iTop, iWidth, iHeight;

		void Initialize2D();
		void PrepareDraw() const;
		void SetCamera(f32 posX, f32 posY);
		void SetViewport(u16 left, u16 top, u16 width, u16 height);

	public:
		Renderer2D();
		virtual ~Renderer2D();

		virtual BOOL Reset();

		virtual void Begin() const;
		virtual BOOL Render(f32 delta);
		virtual void ClearScreen(const PIXEL color) const;
};


}} // namespace


#else // _WII_

	#error "Include 'Renderer2D.h' instead 'platform/wii/WiiRenderer2D.h' directly."

#endif // _WII_
#endif // __WII_RENDERER2D_H__
