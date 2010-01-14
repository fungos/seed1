/*! \file Timeline.h
	\author	Rafael Eduardo Gonchor
	\brief Timeline implementation to be used with ITransformable2D objects
*/

#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#include "Sprite.h"
#include "Point.h"
#include "Keyframe.h"
#include "interface/IEventMovieListener.h"


#define MAX_KEYFRAMES 8192


namespace Seed {


class Movie;


class Timeline
{
	public:
		Timeline();
		~Timeline();

		void Reset();
		void Render(f32 dt);
		void Rewind();

		void AddKeyframe(u32 frame, Keyframe *keyframe);

		void GotoAndPlay(u32 frame);
		void GotoAndPlay(const char *strKeyframeName);

		void SetObject(ITransformable2D *object);
		ITransformable2D *GetObject() const;
		void SetPriority(u32 p);
		f32 GetWidth() const;
		f32 GetHeight() const;
		
		u32 GetCurrentFrame() const;
		const Keyframe *GetCurrentKeyframe() const;

		void SetLocalPosition(f32 posX, f32 posY);
		void SetPosition(f32 posX, f32 posY);
		void SetScale(f32 scaleX, f32 scaleY);
		void SetRotation(f32 rotation);

		void SetParent(Movie *parent);
		Movie *GetParent() const;

		void SetListener(IEventMovieListener *listener);

	private:
		SEED_DISABLE_COPY(Timeline);

		s32 FindNextKeyframe();
		s32 FindPreviousKeyframe();
		s32 FindKeyframeByName(const char *pstrName);

		f32 EaseQuadPercent(f32 time, f32 begin, f32 change, f32 duration, f32 easing);

	private:
		Movie				*pParent;
		ITransformable2D	*pObject;
		IEventMovieListener *pListener;

		f32			fElapsedTime;
		f32			fElapsedKeyframeTime;
		s32			iCurrentFrame;
		s32			iKeyframeFrom;
		s32			iKeyframeTo;

		Point<f32>	ptParentPosition;
		Point<f32>	ptParentLocalPosition;
		Point<f32>	ptParentScale;
		f32			fParentRotation;

		Keyframe *arKeyframes[MAX_KEYFRAMES];
};

} // namespace

#endif // __TIMELINE_H__
