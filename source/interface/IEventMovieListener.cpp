/*! \file IEventMovieListener.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the event movie listener class interface
*/

#include "interface/IEventMovieListener.h"
#include "Log.h"

namespace Seed {

IEventMovieListener::IEventMovieListener()
{
}

IEventMovieListener::~IEventMovieListener()
{
}

INLINE void IEventMovieListener::OnTimelineFrame(const EventMovie *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

INLINE void IEventMovieListener::OnTimelineRestart(const EventMovie *ev)
{
	UNUSED(ev);
	SEED_ABSTRACT_METHOD;
}

} // namespace
