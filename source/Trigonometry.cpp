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

/*! \file Trigonometry.cpp
	\author	Danny Angelo Carminati Grein
	\brief Trigonometry functions
*/

#include "Trigonometry.h"

namespace Seed {

struct SEED_CORE_API sAtan
{
	f32 atan_val;
	f32 atan_delta;
};

sAtan sArcTanTbl[32+1] =
{
	{0.000000000f, 1.272825321f},     // Atan(0.00000) * 256/2pi
	{1.272825321f, 1.270345790f},     // Atan(0.03125) * 256/2pi
	{2.543171111f, 1.265415586f},     // Atan(0.06250) * 256/2pi
	{3.808586697f, 1.258091595f},     // Atan(0.09375) * 256/2pi
	{5.066678293f, 1.248457103f},     // Atan(0.12500) * 256/2pi
	{6.315135396f, 1.236619467f},     // Atan(0.15625) * 256/2pi
	{7.551754863f, 1.222707202f},     // Atan(0.18750) * 256/2pi
	{8.774462065f, 1.206866624f},     // Atan(0.21875) * 256/2pi
	{9.981328688f, 1.189258212f},     // Atan(0.25000) * 256/2pi
	{11.170586901f, 1.170052841f},    // Atan(0.28125) * 256/2pi
	{12.340639741f, 1.149428034f},    // Atan(0.31250) * 256/2pi
	{13.490067775f, 1.127564381f},    // Atan(0.34375) * 256/2pi
	{14.617632156f, 1.104642222f},    // Atan(0.37500) * 256/2pi
	{15.722274378f, 1.080838675f},    // Atan(0.40625) * 256/2pi
	{16.803113053f, 1.056325088f},    // Atan(0.43750) * 256/2pi
	{17.859438141f, 1.031264918f},    // Atan(0.46875) * 256/2pi
	{18.890703059f, 1.005812061f},    // Atan(0.50000) * 256/2pi
	{19.896515121f, 0.980109621f},    // Atan(0.53125) * 256/2pi
	{20.876624742f, 0.954289072f},    // Atan(0.56250) * 256/2pi
	{21.830913814f, 0.928469801f},    // Atan(0.59375) * 256/2pi
	{22.759383615f, 0.902758952f},    // Atan(0.62500) * 256/2pi
	{23.662142567f, 0.877251558f},    // Atan(0.65625) * 256/2pi
	{24.539394125f, 0.852030871f},    // Atan(0.68750) * 256/2pi
	{25.391424996f, 0.827168886f},    // Atan(0.71875) * 256/2pi
	{26.218593881f, 0.802726967f},    // Atan(0.75000) * 256/2pi
	{27.021320848f, 0.778756582f},    // Atan(0.78125) * 256/2pi
	{27.800077430f, 0.755300081f},    // Atan(0.81250) * 256/2pi
	{28.555377511f, 0.732391496f},    // Atan(0.84375) * 256/2pi
	{29.287769007f, 0.710057351f},    // Atan(0.87500) * 256/2pi
	{29.997826358f, 0.688317453f},    // Atan(0.90625) * 256/2pi
	{30.686143811f, 0.667185647f},    // Atan(0.93750) * 256/2pi
	{31.353329458f, 0.646670542f},    // Atan(0.96875) * 256/2pi
	{32.000000000f, 0.626776175f}     // Atan(1.00000) * 256/2pi
};

INLINE f32 Atan2FIdx(f32 y, f32 x);
INLINE f32 AtanFIdx_(f32 x);

INLINE f32 AtanFIdx_(f32 x)
{
	u16 idx;
	f32 val;
	f32 r;

	// 0 Must be <= x <= 1.
	x *= 32.f;
	idx = static_cast<u16>(x);
	r = x - static_cast<f32>(idx);

	val = sArcTanTbl[idx].atan_val + r * sArcTanTbl[idx].atan_delta;

	return val;
}


INLINE f32 Atan2FIdx(f32 y, f32 x)
{
	f32 a;
	f32 b;
	f32 c;
	bool minus;

	if ( x == 0.f && y == 0.f )
	{
		return 0.f;
	}

	if ( x >= 0.f )
	{
		if ( y >= 0.f )
		{
			if ( x >= y )
			{
				// 1
				a = x;
				b = y;
				c = 0.f;
				minus = false;
			}
			else
			{
				// 2
				a = y;
				b = x;
				c = 64.f;
				minus = true;
			}
		}
		else
		{
			if ( x >= -y )
			{
				// -1
				a = x;
				b = -y;
				c = 0.f;
				minus = true;
			}
			else
			{
				// -2
				a = -y;
				b = x;
				c = - 64.f;
				minus = false;
			}
		}
	}
	else
	{
		if ( y >= 0.f )
		{
			if ( -x >= y )
			{
				// 4
				a = -x;
				b = y;
				c = 128.f;
				minus = true;
			}
			else
			{
				// 3
				a = y;
				b = -x;
				c = 64.f;
				minus = false;
			}
		}
		else
		{
			if ( -x >= -y )
			{
				// -4
				a = -x;
				b = -y;
				c = - 128.f;
				minus = false;
			}
			else
			{
				// -3
				a = -y;
				b = -x;
				c = - 64.f;
				minus = true;
			}
		}
	}

	return minus ? c - AtanFIdx_(b/a) : c + AtanFIdx_(b/a);
}

f32 Atan2Deg(f32 y, f32 x)
{
	return MATH_FIDX_TO_DEG(Atan2FIdx(y, x));
}

} // namespace
