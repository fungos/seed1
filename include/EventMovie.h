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

/*! \file EventMovie.h
	\author	Danny Angelo Carminati Grein
	\brief A movie event
*/

#ifndef __EVENTMOVIE_H__
#define __EVENTMOVIE_H__

#include "interface/IEvent.h"

namespace Seed {

class Timeline;
class Keyframe;

class SEED_CORE_API EventMovie : public IEvent
{
	public:
		EventMovie(const Timeline *timeline, const Keyframe *keyframe, u32 frame);
		virtual ~EventMovie();

		virtual const Timeline *GetTimeline() const;
		virtual const Keyframe *GetKeyframe() const;
		virtual u32 GetFrame() const;

		// IObject
		virtual const char *GetObjectName() const;

	private:
		const Timeline	*pTimeline;
		const Keyframe	*pKeyframe;
		u32 iFrame;

	private:
		SEED_DISABLE_COPY(EventMovie);
};

} // namespace

#endif // __EVENTKEYFRAME_H__
