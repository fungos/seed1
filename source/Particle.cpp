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

/*! \file Particle.cpp
	\author	Danny Angelo Carminati Grein
	\brief A particle representation
*/


#include "Particle.h"
#include "Enum.h"


namespace Seed {


Particle::Particle() 
	: bActive(FALSE)
	, ptVelocity(0.0f, 0.0f)
	, fGravity(0.0f)
	, fRadialAccel(0.0f)
	, fTangentialAccel(0.0f)
	, fSpin(0.0f)
	, fSpinDelta(0.0f)
	, fSize(0.0f)
	, fSizeDelta(0.0f)
	, fAge(0.0f)
	, fTerminalAge(0.0f)
	, fColorR(0.0f)
	, fColorG(0.0f)
	, fColorB(0.0f)
	, fColorA(0.0f)
	, fColorDeltaR(0.0f)
	, fColorDeltaG(0.0f)
	, fColorDeltaB(0.0f)
	, fColorDeltaA(0.0f)
{
}

Particle::~Particle()
{
}

INLINE const char *Particle::GetObjectName() const
{
	return "Particle";
}

INLINE int Particle::GetObjectType() const
{
	return Seed::ObjectParticle;
}


} // namespace
