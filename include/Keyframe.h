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

/*! \file Keyframe.h
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#ifndef __KEYFRAME_H__
#define __KEYFRAME_H__

#include "Defines.h"
#include "Point.h"

namespace Seed {

class SEED_CORE_API Keyframe
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
		Point2f 	ptPos;
		Point2f 	ptLocalPos;
		Point2f 	ptScale;

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
