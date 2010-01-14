/*! \file EventMovie.cpp
	\author	Danny Angelo Carminati Grein
	\brief A movie event
*/


#include "EventMovie.h"
#include "Timeline.h"


namespace Seed {


EventMovie::EventMovie(const Timeline *timeline, const Keyframe *keyframe, u32 frame)
	: IEvent(MOVIE, EVENT_MOVIE)
	, pTimeline(timeline)
	, pKeyframe(keyframe)
	, iFrame(frame)
{
}


EventMovie::~EventMovie()
{
}


INLINE const Timeline *EventMovie::GetTimeline() const
{
	return this->pTimeline;
}


INLINE const Keyframe *EventMovie::GetKeyframe() const
{
	return this->pKeyframe;
}


INLINE u32 EventMovie::GetFrame() const
{
	return this->iFrame;
}


INLINE const char *EventMovie::GetObjectName() const
{
	return "EventMovie";
}


} // namespace
