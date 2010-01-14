/*! \file Keyframe.h
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#ifndef __KEYFRAME_H__
#define __KEYFRAME_H__


#include "Defines.h"
#include "Point.h"


namespace Seed {


class Keyframe
{
	public:
		enum
		{
			KEYFRAME_EVENT_NONE = 0,
			KEYFRAME_EVENT_STOP,
			KEYFRAME_EVENT_RESTART,
			KEYFRAME_EVENT_JUMPTOFRAME
		};

	public:
		const char *pstrName;
		f32 		fRotation;
		Point<f32> 	ptPos;
		Point<f32> 	ptLocalPos;
		Point<f32> 	ptScale;

		u32 		iEvent;
		s32 		iFrameToJump;

		f32 		fEasing;

		u8 			iColorR;
		u8 			iColorG;
		u8 			iColorB;
		u8 			iColorA;

		BOOL 		bTween;
		BOOL 		bBlank;

	public:
		Keyframe();
		~Keyframe();

	private:
		SEED_DISABLE_COPY(Keyframe);
};


} // namespace


#endif // __KEYFRAME_H__
