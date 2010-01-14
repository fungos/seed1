/*! \file Trigonometry.h
	\author	Danny Angelo Carminati Grein
	\brief Trigonometry functions
*/


#ifndef __TRIGONOMETRY_H__
#define __TRIGONOMETRY_H__


#include "Defines.h"


#define MATH_FIDX_TO_RAD(fidx) ((fidx) * ((2 * F_PI) / 256.f))
#define MATH_FIDX_TO_DEG(fidx) ((fidx) * (360.f / 256.f))


namespace Seed {


f32 Atan2Deg(f32 y, f32 x);


} // namespace


#endif // __TRIGONOMETRY_H__


