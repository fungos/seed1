/*! \file WiiSprite.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite Implementation
*/

#ifndef __WII_SPRITE_H__
#define __WII_SPRITE_H__

#include "Defines.h"
#include "ResourceManager.h"
#include "interface/IMemoryPool.h"

#if defined(_WII_)

#include "interface/ISprite.h"
#include "SeedInit.h"

#include <math.h>

namespace Seed { namespace WII {

class Sprite : public ISprite
{
	public:
		Sprite();
		virtual ~Sprite();

		// ISprite
		virtual void Reset();
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;
		virtual void ReconfigureFrame();

		// IResource
		//virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		// IUpdatable
		virtual BOOL Update(f32 delta);

		// IRenderable
		virtual void Render(f32 delta);

	private:
		SEED_DISABLE_COPY(Sprite);

	private:
		WiiVec v1, v2, v3, v4;
		WiiTexObj *pData;
		WiiMtx matrix;
		WiiVec p1, p2, p3, p4;
};

}} // namespace

#else // _WII_
	#error "Include 'Sprite.h' instead 'platform/wii/WiiSprite.h' directly."
endif // _WII_
#endif // __WII_SPRITE_H__
