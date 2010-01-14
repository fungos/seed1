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

			if (fLength != 0)
			{
				fLength = 1 / fLength;

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

