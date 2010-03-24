/*! \file IVideo.h
	\author	Danny Angelo Carminati Grein
	\brief Video interface
*/

#ifndef __IVIDEO_H__
#define __IVIDEO_H__

#include "Defines.h"

#include "interface/ITransformable2D.h"
#include "interface/IPlayable.h"

namespace Seed {

class IVideo : public ITransformable2D, public IPlayable
{
	public:
		IVideo();
		virtual ~IVideo();

		//virtual BOOL Load(const char *filename);
		//virtual void Reset();
		//virtual BOOL Unload();

		//virtual BOOL Update(f32 delta);

	private:
		SEED_DISABLE_COPY(IVideo);

	public:
		u32	iDuration;

		u32	iWidth;
		u32	iHeight;
};

} // namespace

#endif // __IVIDEO_H__
