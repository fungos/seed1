/*! \file EventMovie.h
	\author	Danny Angelo Carminati Grein
	\brief A movie event
*/


#ifndef __EVENTMOVIE_H__
#define __EVENTMOVIE_H__


#include "interface/IEvent.h"


namespace Seed {

class Timeline;
class Keyframe;

class EventMovie : public IEvent
{
	public:
		EventMovie(const Timeline *timeline, const Keyframe *keyframe, u32 frame);
		virtual ~EventMovie();

		virtual const Timeline *GetTimeline() const;
		virtual const Keyframe *GetKeyframe() const;
		virtual u32 GetFrame() const;

		// IObject
		virtual const char *GetObjectName() const;

	private:
		const Timeline	*pTimeline;
		const Keyframe	*pKeyframe;
		u32 iFrame;

	private:
		SEED_DISABLE_COPY(EventMovie);
};


} // namespace


#endif // __EVENTKEYFRAME_H__
