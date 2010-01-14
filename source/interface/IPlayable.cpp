/*! \file IPlayable.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Playable interface
*/

#include "interface/IPlayable.h"

namespace Seed {


IPlayable::IPlayable()
	: nState(PlayableStopped)
{
}

IPlayable::~IPlayable()
{
}

INLINE void IPlayable::Play()
{
	nState = PlayablePlaying;
}

INLINE BOOL IPlayable::IsPlaying() const
{
	return (nState == PlayablePlaying);
}

INLINE void IPlayable::Stop()
{
	nState = PlayableStopped;
}

INLINE BOOL IPlayable::IsStopped() const
{
	return (nState == PlayableStopped);
}

INLINE void IPlayable::Pause()
{
	nState = PlayablePaused;
}

INLINE BOOL IPlayable::IsPaused() const
{
	return (nState == PlayablePaused);
}

INLINE BOOL IPlayable::IsSeekable() const
{
	return FALSE;
}

INLINE void IPlayable::Seek(f32 pos)
{
	UNUSED(pos);
}

} // namespace
