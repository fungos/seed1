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

/*! \file Point.h
	\author Rafael Eduardo Gonchor
	\brief Point utils
*/


#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>
#include <stdlib.h>


namespace Seed {

// was fixed32
template <class TYPE> class Point
{
	public:
		TYPE x;
		TYPE y;

		Point(TYPE fX = 0, TYPE fY = 0) 
			: x(fX)
			, y(fY)
		{
		}

		Point(const Point<TYPE> &point)
			: x(0)
			, y(0)
		{
			LoadPoint(point);
		}

		virtual ~Point()
		{
		}

		Point<TYPE> &operator=(const Point<TYPE> &point)
		{
			x = point.x;
			y = point.y;

			return *this;
		}

		void operator+=(const Point<TYPE> &point)
		{
			x += point.x;
			y += point.y;
		}

		Point operator+(const Point<TYPE> &point) const
		{
			Point<TYPE> ret(*this);

			ret += point;

			return ret;
		}

		void operator+=(TYPE f)
		{
			x += f;
			y += f;
		}

		Point<TYPE> operator+(TYPE f) const
		{
			Point<TYPE> ret(*this);

			ret += f;

			return ret;
		}

		void operator-=(const Point<TYPE> &point)
		{
			x -= point.x;
			y -= point.y;
		}

		Point<TYPE> operator-(const Point<TYPE> &point) const
		{
			Point<TYPE> ret(*this);

			ret -= point;

			return ret;
		}

		void operator-=(TYPE f)
		{
			x -= f;
			y -= f;
		}

		Point operator-(TYPE f) const
		{
			Point<TYPE> ret(*this);

			ret -= f;

			return ret;
		}

		virtual void operator*=(TYPE f)
		{
			x *= f;
			y *= f;
		}

		Point<TYPE> operator*(TYPE f) const
		{
			Point<TYPE> ret(*this);

			ret *= f;

			return ret;
		}

		virtual void operator*=(const Point<TYPE> &point)
		{
			x *= point.x;
			y *= point.y;
		}

		Point<TYPE> operator*(const Point &point) const
		{
			Point<TYPE> ret(*this);

			ret *= point;

			return ret;
		}

		virtual void operator/=(TYPE f)
		{
			(*this) *= 1/f;
		}

		Point<TYPE> operator/(TYPE f)
		{
			Point<TYPE> ret(*this);

			ret *= 1 / f;

			return ret;
		}

		BOOL operator==(const Point<TYPE> &point) const
		{
			return ((x == point.x) && (y == point.y));
		}

		BOOL operator!=(const Point<TYPE> &point) const
		{
			return (!(x == point.x) || !(y == point.y));
		}

		Point& LoadPoint(const Point<TYPE> &point)
		{
			*this = point;
			return *this;
		}

		TYPE Distance(const Point<TYPE> &point)
		{
			*this -= point;
			*this *= *this;
			return FAST_SQRT(x+y);
		}

		TYPE Dot(const Point<TYPE> &point) const
		{
			return ((x * point.x) + (y * point.y));
		}

		Point<TYPE> & Cross()
		{
			TYPE new_x = y;
			TYPE new_y = -x;

			x = new_x;
			y = new_y;

			return *this;
		}

		TYPE SquareLength() const
		{
			return ((x * x) + (y * y));
		}

		TYPE Length() const
		{
			return sqrt(SquareLength());
		}

		Point<TYPE> & Normalize()
		{
			TYPE fLength = Length();

			if (fLength != 0.0f)
			{
				fLength = 1.0f / fLength;

				(*this) *= fLength;
			}

			return *this;
		}

		Point<TYPE> & Lerp(const Point<TYPE> &point, f32 f)
		{
			x += (point.x-x) * f;
			y += (point.y-y) * f;

			return *this;
		}

		TYPE Angle(const Point<TYPE> *p = NULL) const
		{
			if (p)
			{
				Point<TYPE> s = *this;
				Point<TYPE> t = *p;

				s.Normalize(); 
				t.Normalize();
				return acosf(s.Dot(t));
			}
			else 
				return atan2f(y, x);
		}
};


} // namespace


#endif // _POINT_H__

