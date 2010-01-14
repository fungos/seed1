/*! \file Number.h
	\author Rafael Eduardo Gonchor
	\brief Numeber util
*/

#ifndef __NUMBER_H__
#define __NUMBER_H__


namespace Seed {


class Number
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
};


} // namespace


#endif // __NUMBER_H__

