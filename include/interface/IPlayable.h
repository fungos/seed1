/*! \file IPlayable.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Playable interface
*/

#include "Defines.h"


namespace Seed {


enum ePlayableState
{
	PlayableStopped,
	PlayablePlaying,
	PlayablePaused,
};

class IPlayable
{
	public:
		IPlayable();
		virtual ~IPlayable();

		/// Play a playable object. Play starts playing if the object was stopped or resume playing if it was paused.
		virtual void Play();
		virtual BOOL IsPlaying() const;

		/// Stop a playable object. Stops a playing object, by stopping we are Reseting the object state to initial state.
		virtual void Stop();
		virtual BOOL IsStopped() const;

		/// Pause a playable object. Pause freezes the object updating of its internal state and can be resumed by calling Play.
		virtual void Pause();
		virtual BOOL IsPaused() const;

		/// Check if this playable object is seekable.
		virtual BOOL IsSeekable() const;

		/// Seek to a position if it is seekable. To be defined: the pos must be a position in time of a simulation step per seconds unity.
		virtual void Seek(f32 pos);

	protected:
		ePlayableState	nState;
	
	private:
		SEED_DISABLE_COPY(IPlayable);
};


} // namespace
