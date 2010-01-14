/*! \file Movie.h
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#ifndef __MOVIE_H__
#define __MOVIE_H__


#include "Timeline.h"
#include "Array.h"


namespace Seed {


#define MAX_TIMELINES 36


class Movie : public ITransformable2D
{
	public:
		Movie();
		virtual ~Movie();

		void AddTimeline(Timeline *pTimeline);
		void Play();
		void Stop();
		void Rewind();
		void Reset();

		// IRenderable
		virtual void Update();
		virtual void Render(f32 delta);

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(Movie);

	private:
		BOOL	bPlaying;
		Array<Timeline *, MAX_TIMELINES> arTimelines;
};


} // namespace


#endif // __MOVIE_H__
