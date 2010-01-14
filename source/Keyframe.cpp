/*! \file Keyframe.cpp
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#include "Keyframe.h"


namespace Seed {


Keyframe::Keyframe()
	: pstrName("")
	, fRotation(0.0f)
	, ptPos(0.0f, 0.0f)
	, ptLocalPos(0.0f, 0.0f)
	, ptScale(1.0f, 1.0f)
	, iEvent(KEYFRAME_EVENT_NONE)
	, iFrameToJump(-1)
	, fEasing(0.0f)
	, iColorR(255)
	, iColorG(255)
	, iColorB(255)
	, iColorA(255)
	, bTween(FALSE)
	, bBlank(FALSE)
{
}


Keyframe::~Keyframe()
{
}


} // namespace
