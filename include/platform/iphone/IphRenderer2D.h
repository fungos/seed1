/*! \file IphRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief Renderer2D Iphone Implementation
*/


#ifndef __IPH_RENDERER2D_H__
#define __IPH_RENDERER2D_H__

#include "Renderer.h"
#include "interface/IRenderer2D.h"

#ifdef _IPHONE_


namespace Seed { namespace iPhone {


class Renderer2D : public Renderer, public Seed::IRenderer2D
{
	public:
		Renderer2D();
		virtual ~Renderer2D();

		virtual void Begin() const;
		virtual void End() const;
		virtual void ClearScreen(const PIXEL color) const;


	private:
		Renderer2D(const Renderer2D &);
		Renderer2D &operator=(const Renderer2D &);

		void Initialize2D();
		void SetCamera(f32 posX, f32 posY);
		void SetViewport(u16 left, u16 top, u16 width, u16 height);
		void Orthogonal(f32 width, f32 height);
};


}} // namespace


#else // _IPHONE_

	#error "Include 'Renderer2D.h' instead 'platform/iphone/IphRenderer2D.h' directly."

#endif // _IPHONE_
#endif // __IPH_RENDERER2D_H__
