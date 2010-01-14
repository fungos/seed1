/*! \file IEventMovieListener.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the event movie listener class interface
*/


#ifndef __IEVENT_MOVIE_LISTENER_H__
#define __IEVENT_MOVIE_LISTENER_H__


#include "interface/IEventListener.h"
#include "interface/IEvent.h"


namespace Seed {


class EventMovie;
class Timeline;


class IEventMovieListener : public IEventListener
{
	public:
		IEventMovieListener();
		virtual ~IEventMovieListener();

		virtual void OnTimelineFrame(const EventMovie *ev);
		virtual void OnTimelineRestart(const EventMovie *ev);

	private:
		SEED_DISABLE_COPY(IEventMovieListener);
};


} // namespace


#endif // __IEVENT_MOVIE_LISTENER_H__

