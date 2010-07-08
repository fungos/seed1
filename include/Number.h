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

/*! \file Number.h
	\author Rafael Eduardo Gonchor
	\brief Numeber util
*/

#ifndef __NUMBER_H__
#define __NUMBER_H__

namespace Seed {

class SEED_CORE_API Number
{
	public:
		static u32 Floor(f32 number)
		{
			u32 intPart = (u32)number;
			return intPart;
		}

		static u32 Ceil(f32 number)
		{
			u32 intPart = (u32)number;

			if (number - intPart > 0)
				return intPart + 1;

			return intPart;
		}

		static u32 Round(f32 number)
		{
			u32 intPart = (u32)number;

			if (number - (f32)intPart <= 0.5)
				return intPart;

			return intPart + 1;
		}

		static f32 Abs(f32 number)
		{
			return number >= 0.0f ? number : -number;
		}

		static int Abs(int number)
		{
			return number >= 0 ? number: -number;
		}
};

} // namespace

#endif // __NUMBER_H__

