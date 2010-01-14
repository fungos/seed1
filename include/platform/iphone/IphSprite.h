/*! \file IphSprite.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite Iphone Implementation
*/

#ifndef __IPH_SPRITE_H__
#define __IPH_SPRITE_H__

#ifdef _IPHONE_

#include "Defines.h"
#include "interface/IImage.h"
#include "interface/ISprite.h"
#include "SeedInit.h"
#include "SpriteObject.h"
#include "Image.h"
#include "ResourceManager.h"


#include <math.h>


namespace Seed { namespace iPhone {


class Sprite : public ISprite
{
	public:
		Sprite();
		virtual ~Sprite();

		// ISprite
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		// IUpdatable
		virtual void Update();

		// IRenderable
		virtual void Render(f32 delta);


	private:
		Sprite(const Sprite &);
		Sprite &operator=(const Sprite &);

		virtual void ReconfigureFrame();


	private:
		const void	*pData;

		GLfloat vertices[8];
		GLfloat coords[8];
};


}} // namespace


#else // _IPHONE_

	#error "Include 'Sprite.h' instead 'platform/iphone/IphSprite.h' directly."

#endif // _IPHONE_
#endif // __IPH_SPRITE_H__
