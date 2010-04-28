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

/*! \file Particle.h
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Sprite.h"
#include "Point.h"

namespace Seed {

class SEED_CORE_API Particle : public Sprite
{
	friend class ParticleEmitter;

	public:
		Particle();
		virtual ~Particle();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		BOOL	bActive;
		Point<f32>	ptVelocity;

		f32		fGravity;
		f32		fRadialAccel;
		f32		fTangentialAccel;

		f32		fSpin;
		f32		fSpinDelta;

		f32		fSize;
		f32		fSizeDelta;

		f32		fAge;
		f32		fTerminalAge;

		f32		fColorR;
		f32		fColorG;
		f32		fColorB;
		f32		fColorA;

		f32		fColorDeltaR;
		f32		fColorDeltaG;
		f32		fColorDeltaB;
		f32		fColorDeltaA;

		//ITransformable2D	*pObject;

	private:
		SEED_DISABLE_COPY(Particle);
};

} // namespace

#endif // __PARTICLE_H__
