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

/*! \file MathUtil.h
	\author	Thiago C. Moraes
			Danny Angelo Carminati Grein
	\brief Math utils
*/

#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include <math.h>

#define LIB_SQRT sqrt
#define LIB_SIN	sin
#define LIB_COS	cos

// Forward declarations
namespace Seed {

template <typename T> class Vector3;
template <typename T> class Matrix4x4;
template <typename T> class Quaternion;

}

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"


namespace Seed {

// Declare a global constant for pi and a few multiples.
// Math constants
const f32 kPi = 3.14159265358979323846f; // M_PI
const f32 k2Pi = kPi * 2.0f;

const f32 kPiOver2 = kPi / 2.0f; // M_PI_2 1.57079632679489661923f
const f32 kPiOver4 = kPi / 4.0f; // M_PI_4 0.785398163397448309616f
const f32 k1OverPi = 1.0f / kPi; // M_1_PI 0.318309886183790671538f
const f32 k2OverPi = 2.0f / kPi; // M_2_PI 0.636619772367581343076f
const f32 k1Over2Pi = 1.0f / k2Pi;
const f32 kPiOver180 = kPi / 180.0f;
const f32 k180OverPi = 180.0f / kPi;

// "Wrap" an angle in range -pi...pi by adding the correct multiple of 2 pi
template <typename T>
T WrapPi(T theta);

// "Safe" inverse trig functions
template <typename T>
T SafeAcos(T x);

// Set the Euler angle triple to its "canonical" value
template <typename T>
void CanonizeEulerAngles(T &roll, T &pitch, T &yaw);

// Convert between degrees and radians
template <typename T>
inline T DegToRad(T deg)
{
	return deg * kPiOver180;
}

template <typename T> inline T RadToDeg(T rad)
{
	return rad * k180OverPi;
}

// Convert between "field of view" and "zoom".
// The FOV angle is specified in radians.
template <typename T> inline T FovToZoom(T fov)
{
	return 1.0f / LIB_TAN(fov * 0.5f);
}

template <typename T> inline T ZoomToFov(T zoom)
{
	return 2.0f * LIB_ATAN(1.0f / zoom);
}


// --------------------------------------------------------------------------
// WrapPi
//
// "Wrap" an angle in range -pi...pi by adding the correct multiple
// of 2 pi
// --------------------------------------------------------------------------
template <typename T>
inline T WrapPi(T theta)
{
	theta += kPi;
	theta -= LIB_FLOOR(theta * k1Over2Pi) * k2Pi;
	theta -= kPi;

	return theta;
}

// --------------------------------------------------------------------------
// safeAcos
//
// Same as acos(x), but if x is out of range, it is "clamped" to the nearest
// valid value. The value returned is in range 0...pi, the same as the
// standard C acos() function
// --------------------------------------------------------------------------
template <typename T>
inline T SafeAcos(T x)
{
	// Check limit conditions
	if (x <= -1.0)
		return kPi;
	
	if (x >= 1.0)
		return 0.0;
	
	// value is in the domain - use standard C function
	return LIB_ACOS(x);
}

// --------------------------------------------------------------------------
// canonizeEulerAngles
//
// Set the Euler angle triple to its "canonical" value.  This does not change
// the meaning of the Euler angles as a representation of Orientation in 3D,
// but if the angles are for other purposes such as angular velocities, etc,
// then the operation might not be valid.
// --------------------------------------------------------------------------
template <typename T>
inline void CanonizeEulerAngles(T &roll, T &pitch, T &yaw)
{
	// First, wrap pitch in range -pi ... pi
	pitch = WrapPi(pitch);
	
	// Now, check for "the back side" of the matrix, pitch outside
	// the canonical range of -pi/2 ... pi/2
	if (pitch < -kPiOver2)
	{
		roll += kPi;
		pitch = -kPi - pitch;
		yaw += kPi;
	}
	else if (pitch > kPiOver2)
	{
		roll += kPi;
		pitch = kPi - pitch;
		yaw += kPi;
	}
	
	// OK, now check for the Gimbal lock case (within a slight
	// tolerance)
	if (LIB_FABS(pitch) > kPiOver2 - 1e-4)
	{
		// We are in gimbal lock.  Assign all rotation
		// about the vertical axis to heading
		yaw += roll;
		roll = 0.0;
	}
	else
	{
		// Not in gimbal lock.  Wrap the bank angle in
		// canonical range
		roll = WrapPi(roll);
	}
	
	// Wrap heading in canonical range
	yaw = WrapPi(yaw);
}

} // namespace

#endif // __MATH_UTIL_H__
