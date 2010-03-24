/*! \file IPlayable.h
	\author	Danny Angelo Carminati Grein
	\brief Playable interface - animations should use it (video, sprite, movies, etc.)
*/

#ifndef __IPLAYABLE_H__
#define __IPLAYABLE_H__

#include "Defines.h"
#include "Enum.h"

namespace Seed {

class IPlayable
{
	public:
		IPlayable();
		virtual ~IPlayable();

		virtual void GotoAndPlay(u32 frame);
		virtual void GotoAndStop(u32 frame);
		virtual void Play();
		virtual BOOL IsPlaying() const;

		virtual void Stop();
		virtual BOOL IsStopped() const;

		virtual void Pause();
		virtual BOOL IsPaused() const;

		virtual void SetLoop(BOOL loop);
		virtual BOOL IsLoop() const;

		virtual void Seek(f32 d);
		virtual BOOL IsSeekable() const;

		virtual void Rewind();
		virtual BOOL IsFinished() const;

		virtual u32 GetNumFrames() const;
		virtual f32 GetFrameRate() const;

	private:
		SEED_DISABLE_COPY(IPlayable);

	protected:
		f32 fFrameRate;
		u32 iTotalFrames;
		ePlayableState	nState;
		BOOL bFinished;
		BOOL bLoop;
};

} // namespace

#endif // __IPLAYABLE_H__
