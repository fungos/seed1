/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

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
#undef GetObject
#undef GetObjectW
#undef GetObjectA

#define MAX_KEYFRAMES 8192

namespace Seed {

class Movie;

class SEED_CORE_API Timeline
{
	public:
		Timeline();
		~Timeline();

		void Reset();
		void Update();
		void Rewind();

		void AddKeyframe(u32 frame, Keyframe *keyframe);

		void GotoAndPlay(u32 frame);
		void GotoAndPlay(const char *strKeyframeName);

		void SetObject(ISceneObject *object);
		ISceneObject *GetObject() const;
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
		ISceneObject		*pObject;
		IEventMovieListener *pListener;

		f32			fElapsedTime;
		f32			fElapsedKeyframeTime;
		s32			iCurrentFrame;
		s32			iKeyframeFrom;
		s32			iKeyframeTo;

		Point2f		ptParentPosition;
		Point2f		ptParentLocalPosition;
		Point2f		ptParentScale;
		f32			fParentRotation;

		Keyframe *arKeyframes[MAX_KEYFRAMES];
};

} // namespace

#endif // __TIMELINE_H__
