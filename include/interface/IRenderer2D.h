/*! \file IRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Renderer 2D class interface
*/

#ifndef __IRENDERER2D_H__
#define __IRENDERER2D_H__


#include "Defines.h"


namespace Seed {


class IRenderer2D
{
	public:
		IRenderer2D();
		virtual ~IRenderer2D();

		virtual void ClearScreen(const PIXEL color) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill = FALSE) const;

	private:
		SEED_DISABLE_COPY(IRenderer2D);
};


} // namespace


#endif // __IRENDERER2D_H__
