/*! \file Movie.cpp
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#include "Movie.h"

namespace Seed {

Movie::Movie()
	: ITransformable2D()
	, bPlaying(TRUE)
	, arTimelines()
{
}

Movie::~Movie()
{
	this->Reset();
}

INLINE void Movie::AddTimeline(Timeline *pTimeline)
{
	arTimelines.Add(pTimeline);
	pTimeline->SetParent(this);
/*
	pTimeline->SetLocalPosition(GetLocalX(), GetLocalY());
	pTimeline->SetPosition(GetX(), GetY());
	pTimeline->SetScale(GetScaleX(), GetScaleY());
	pTimeline->SetRotation(GetRotation());
*/
}

INLINE void Movie::Update(f32 delta)
{
	UNUSED(delta);
}

INLINE void Movie::Play()
{
	this->bPlaying = TRUE;
}

INLINE void Movie::Stop()
{
	this->bPlaying = FALSE;
}

INLINE void Movie::Rewind()
{
	for (u32 i = 0; i < arTimelines.Size(); i++)
	{
		arTimelines[i]->Rewind();
	}
}

INLINE void Movie::Reset()
{
	for (u32 i = 0; i < arTimelines.Size(); i++)
		arTimelines[i]->Reset();

	arTimelines.Truncate();
}

INLINE void Movie::Render(f32 delta)
{
	if (!this->bPlaying)
		return;

	for (u32 i = 0; i < arTimelines.Size(); i++)
	{
		if (bTransformationChanged)
		{
			arTimelines[i]->SetLocalPosition(this->GetLocalX(), this->GetLocalY());
			arTimelines[i]->SetPosition(this->GetX(), this->GetY());
			arTimelines[i]->SetScale(this->GetScaleX(), this->GetScaleY());
			arTimelines[i]->SetRotation(this->GetRotation());
		}

		arTimelines[i]->Render(delta);
	}

	UNUSED(delta);
}

INLINE const char *Movie::GetObjectName() const
{
	return "Movie";
}

INLINE int Movie::GetObjectType() const
{
	return Seed::ObjectMovie;
}

} // namespace
