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

/*! \file Rand.cpp
	\author	Danny Angelo Carminati Grein
	\brief Pseudo random number generator
*/

#include "Rand.h"
#include <math.h>

namespace Seed {

SEED_SINGLETON_DEFINE(Rand);

Rand::Rand()
	: iSeed(0)
{
}

Rand::~Rand()
{
}

INLINE void Rand::Initialize(u32 seed)
{
	if (!seed)
		iSeed = RAND;
	else
		iSeed = seed;
}

INLINE u32 Rand::Get(u32 max)
{
	return this->Get(0, max);
}

INLINE f32 Rand::Get(f32 max)
{
	return this->Get(0, max);
}

INLINE u32 Rand::Get(u32 min, u32 max)
{

	iSeed = 214013 * iSeed + 2531011;
	return min + (iSeed ^ iSeed >> 15) % (max - min);

	//return min + (RAND >> 15) % (max - min);
}

INLINE f32 Rand::Get(f32 min, f32 max)
{

	iSeed = 214013 * iSeed + 2531011;
	return min + (iSeed >> 16) * (1.0f / 65535.0f) * (max - min);

	//return min + (RAND >> 16) * (1.0f / 65535.0f) * (max - min);
}

} // namespace
