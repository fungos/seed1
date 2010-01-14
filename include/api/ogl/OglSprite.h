/*! \file OglSprite.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite OpenGL Implementation
*/


#ifndef __OGL_SPRITE_H__
#define __OGL_SPRITE_H__

#include "Defines.h"

#if defined(_OGL_)

#include "interface/ISprite.h"
#include "SeedInit.h"
#include "MathUtil.h"


#include <math.h>


namespace Seed { namespace OGL {


class Sprite : public ISprite
{
	public:
		Sprite();
		virtual ~Sprite();

		// ISprite
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		// IUpdatable
		virtual void Update(f32 delta);

		// IRenderable
		virtual void Render(f32 delta);

	private:
		SEED_DISABLE_COPY(Sprite);
		virtual void SetupBlendingOperation();

	private:
		Vector3f vert[4];

#if SEED_ENABLE_DEPTH_TEST
		GLfloat vertices[12];
#else
		GLfloat vertices[8];
#endif // SEED_ENABLE_DEPTH_TEST
		GLfloat coords[8];
};


}} // namespace


#else // _OGL_

	#error "Include 'Sprite.h' instead 'api/ogl/OglSprite.h' directly."

#endif // _OGL_
#endif // __OGL_SPRITE_H__
