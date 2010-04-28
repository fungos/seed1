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

/*! \file Vector3.h
	\author	Rafael Eduardo Gonchor
			Thiago C. Moraes
			Danny Angelo Carminati Grein
	\brief Vector3
*/

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <math.h>

namespace Seed {

template <typename T> class Vector3
{
	public:
		// Constructors
		inline Vector3()
			: x(0)
			, y(0)
			, z(0)
		{
		}

		inline Vector3(T x, T y, T z)
			: x(x)
			, y(y)
			, z(z)
		{
		}

		inline ~Vector3()
		{
		}

		bool operator==(const Vector3<T> &v) const;
		bool operator!=(const Vector3<T> &v) const;
		Vector3<T> operator-() const;

		// Vector operations
		Vector3<T> operator+(const Vector3<T> &v) const;
		Vector3<T> operator-(const Vector3<T> &v) const;
		Vector3<T> operator*(T s) const;
		Vector3<T> operator/(T s) const;

		// Combined assignment operators to conform to C notation convention
		Vector3<T> &operator+=(const Vector3<T> &v);
		Vector3<T> &operator-=(const Vector3<T> &v);
		Vector3<T> &operator*=(T s);
		Vector3<T> &operator/=(T s);

		// Accessor.  This allows to use the vector object like an array of T. For example:
		// Vector3<float> v (...);
		// float f = v[1]; // access to y
		inline operator const T*()
		{
			return v;
		}

		BOOL IsZero();
		Vector3<T> & Normalize();
		T Distance(const Vector3<T> &v);
		T DistanceSquared(const Vector3<T> &v);
		T Dot(const Vector3<T> &v) const;
		T LengthSquared() const;
		T Length() const;
		Vector3<T> & Lerp(const Vector3<T> &v, f32 f);
		T Angle(const Vector3<T> *v = NULL) const;

		union
		{
			struct
			{
				T x;
				T y;
				T z;
			};

			T v[3];
		};
};

template <typename T>
inline bool Vector3<T>::operator==(const Vector3<T> &v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z));
}

template <typename T>
inline bool Vector3<T>::operator!=(const Vector3<T> &v) const
{
	return ((x != v.x) || (y != v.y) || (z != v.z));
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

// Vector operations
template <typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T> &v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T> &v) const
{
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator*(T s) const
{
	return Vector3<T>(x * s, y * s, z * s);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator/(T s) const
{
	ASSERT(s != 0);

	T oneOverS = (T)((T)1.0 / (T)s); // assert for divide by zero
	return Vector3<T>(x * oneOverS, y * oneOverS, z * oneOverS);
}

// Combined assignment operators to conform to C notation convention
template <typename T>
inline Vector3<T> &Vector3<T>::operator+=(const Vector3<T> &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

template <typename T>
inline Vector3<T> &Vector3<T>::operator-=(const Vector3<T> &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

template <typename T>
inline Vector3<T> &Vector3<T>::operator*=(T s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

template <typename T>
inline Vector3<T> &Vector3<T>::operator/=(T s)
{
	ASSERT(s != 0);

	T oneOverS = 1.0 / s; // assert divide by zero!
	x *= oneOverS;
	y *= oneOverS;
	z *= oneOverS;

	return *this;
}

// --------------------------------------------------------------------------
// Return true if is zero vector.
// --------------------------------------------------------------------------
template <typename T>
inline BOOL Vector3<T>::IsZero()
{
		return (x == 0.0) && (y == 0.0) && (z == 0.0);
}

// --------------------------------------------------------------------------
// Set vector length to 1.
// --------------------------------------------------------------------------
template <typename T>
inline Vector3<T> &Vector3<T>::Normalize()
{
	T len = ((x * x) + (y * y) + (z * z));

	if (len > 0.0)
	{
		len = 1.0f / LIB_SQRT(len);
		x *= len;
		y *= len;
		z *= len;
	}

	return *this;
}

template <typename T>
inline T Vector3<T>::Distance(const Vector3<T> &v)
{
	*this -= v;
	*this *= *this;
	return LIB_SQRT(x + y + z);
}

template <typename T>
inline T Vector3<T>::DistanceSquared(const Vector3<T> &v)
{
	*this -= v;
	*this *= *this;
	return (x + y + z);
}

template <typename T>
inline T Vector3<T>::Dot(const Vector3<T> &v) const
{
	return ((x * v.x) + (y * v.y) + (z * v.z));
}

template <typename T>
inline T Vector3<T>::LengthSquared() const
{
	return ((x * x) + (y * y) + (z * z));
}

template <typename T>
inline T Vector3<T>::Length() const
{
	return LIB_SQRT((x * x) + (y * y) + (z * z));
}

template <typename T>
inline Vector3<T> &Vector3<T>::Lerp(const Vector3<T> &v, f32 f)
{
	x += (v.x - x) * f;
	y += (v.y - y) * f;
	z += (v.z - z) * f;

	return *this;
}

template <typename T>
inline T Vector3<T>::Angle(const Vector3<T> *v) const
{
	if (v)
	{
		Vector3<T> s = *this, t = *v;

		s.Normalize();
		t.Normalize();
		return acosf(s.Dot(t));
	}
	else
	{
		return atan2f(y, x);
	}
}

//
// Nonmember Vector3 functions
//
template <typename T>
inline Vector3<T> operator*(T k, Vector3<T> v)
{
	return Vector3<T>(k * v.x, k * v.y, k * v.z);
}

template <typename T> T LengthSquared(const Vector3<T> &v)
{
	return (v.x * v.x) +  (v.y * v.y) +  (v.z * v.z);
}

template <typename T> T Length(const Vector3<T> &v)
{
	return LIB_SQRT((v.x * v.x) +  (v.y * v.y) +  (v.z * v.z));
}

template <typename T> T DotProduct(const Vector3<T> &a, const Vector3<T> &b)
{
	return ((a.x * b.x) +  (a.y * b.y) +  (a.z * b.z));
}

template <typename T> Vector3<T> CrossProduct(const Vector3<T> &a, const Vector3<T> &b)
{
	return Vector3<T>((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

template <typename T> Vector3<T> ComputeNormal(const Vector3<T> &p1, const Vector3<T> &p2, const Vector3<T> &p3)
{
	Vector3<T> vec1(p1 - p2);
	Vector3<T> vec2(p1 - p3);

	Vector3<T> result(CrossProduct(vec1, vec2));
	result.Normalize();

	return result;
}

template <typename T> T Distance(const Vector3<T> &a, const Vector3<T> &b)
{
	T dx = a.x - b.x;
	T dy = a.y - b.y;
	T dz = a.z - b.z;

	return LIB_SQRT((dx * dx) + (dy * dy) + (dz * dz));
}

template <typename T> T DistanceSquared(const Vector3<T> &a, const Vector3<T> &b)
{
	T dx = a.x - b.x;
	T dy = a.y - b.y;
	T dz = a.z - b.z;

	return ((dx * dx) + (dy * dy) + (dz * dz));
}

// Predefined Vector3 types
typedef Vector3<f32> Vector3f;
typedef Vector3<double> Vector3d;

// We provide a global constant zero vector
static const Vector3f ZeroVectorf(0.0f, 0.0f, 0.0f);
static const Vector3d ZeroVectord(0.0, 0.0, 0.0);

} // namespace

#endif // __VECTOR_H__
