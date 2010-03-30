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

/*! \file Quaternion.h
	\author	Thiago C. Moraes
			Danny Angelo Carminati Grein
	\brief Quaternion
*/

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

/*
#include "Math.h"

template <typename T> class Quaternion
{
	public:
		// Constructors
		Quaternion()
		{
		}
		
		Quaternion(T w, T x, T y, T z)
			: w(w)
			, x(x)
			, y(y)
			, z(z)
		{
		}
		
		// Public interface
		void Identity();
		void Normalize();
		void ComputeW();
		void Rotate(Vector3<T> &v) const;
		
		void FromMatrix(const Matrix4x4<T> &m);
		
		// Quaternion <-> Euler conversions; XYZ rotation order; angles in radians
		void FromEulerAngles(T x, T y, T z);
		void ToEulerAngles(T &x, T &y, T &z) const;
		
		T RotationAngle() const;
		Vector3<T> RotationAxis() const;
		
		// Quaternion operations
		Quaternion<T> operator+(const Quaternion<T> &q) const;
		Quaternion<T> &operator+=(const Quaternion<T> &q);
		
		Quaternion<T> operator-(const Quaternion<T> &q) const;
		Quaternion<T> &operator-=(const Quaternion<T> &q);
		
		Quaternion<T> operator*(const Quaternion<T> &q) const;
		Quaternion<T> &operator*=(const Quaternion<T> &q);
		
		Quaternion<T> operator*(T k) const;
		Quaternion<T> &operator*=(T k);
		
		Quaternion<T> operator*(const Vector3<T> &v) const;
		Quaternion<T> &operator*=(const Vector3<T> &v);
		
		Quaternion<T> operator/(T k) const;
		Quaternion<T> &operator/=(T k);
		
		Quaternion<T> operator~() const; // Quaternion conjugate
		Quaternion<T> operator-() const; // Quaternion negation
		
		// Member variables
		
		// The 4 values of the quaternion.  Normally, it will not
		// be necessary to manipulate these directly.  However,
		// we leave them public, since prohibiting direct access
		// makes some operations, such as file I/O, unnecessarily
		// complicated.
		union
		{
			struct
			{
				T w, x, y, z;
			};
			
			T q[4];
		};
};


// Predefined Quaternion types
typedef Quaternion<f32> Quaternionf;
typedef Quaternion<double> Quaterniond;

// A global "identity" quaternion constant
static const Quaternionf QuaternionIdentityf(1.0f, 0.0f, 0.0f, 0.0f);
static const Quaterniond QuaternionIdentityd(1.0f, 0.0f, 0.0f, 0.0f);

//
// Nonmember Quaternion functions
//
template <typename T> Quaternion<T> operator*(T k, const Quaternion<T> &q);
template <typename T> T 			DotProduct(const Quaternion<T> &a, const Quaternion<T> &b);
template <typename T> Quaternion<T> Conjugate(const Quaternion<T> &q);
template <typename T> Quaternion<T> Inverse(const Quaternion<T> &q);
template <typename T> Quaternion<T> RotationQuaternion(Axis axis, T theta);
template <typename T> Quaternion<T> RotationQuaternion(const Vector3<T> &axis, T theta);
template <typename T> Quaternion<T> Log(const Quaternion<T> &q);
template <typename T> Quaternion<T> Exp(const Quaternion<T> &q);
template <typename T> Quaternion<T> Pow(const Quaternion<T> &q, T exponent);
template <typename T> Quaternion<T> Slerp(const Quaternion<T> &q0, const Quaternion<T> &q1, T t);
template <typename T> Quaternion<T> Squad(const Quaternion<T> &q0, const Quaternion<T> &qa, const Quaternion<T> &qb, const Quaternion<T> &q1, T t);
template <typename T> inline void 	Intermediate(const Quaternion<T> &qprev, const Quaternion<T> &qcurr, const Quaternion<T> &qnext, Quaternion<T> &qa, Quaternion<T> &qb);



/////////////////////////////////////////////////////////////////////////////
// class Quaternion<T> implementation.
/////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------
// Set to identity
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::Identity()
{
	w = 1.0;
	x = y = z = 0.0;
}

// --------------------------------------------------------------------------
// "Normalize" a quaternion.  Note that normally, quaternions
// are always normalized (within limits of numerical precision).
//
// This function is provided primarily to combat floating point "error
// creep", which can occur when many successive quaternion operations
// are applied.
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::Normalize()
{
	// Compute magnitude of the quaternion
	T mag = (w * w) + (x * x) + (y * y) + (z * z);
	
	// Check for bogus length, to protect against divide by zero
	if (mag > 0.0)
	{
		// Normalize it
		T oneOverMag = 1.0 / LIB_SQRT(mag);
		
		w *= oneOverMag;
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	}
}

// --------------------------------------------------------------------------
// Compute the W component of a unit quaternion given its x,y,z components.
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::ComputeW()
{
	T t = 1.0 - (x * x) - (y * y) - (z * z);
	
	if (t < 0.0)
		w = 0.0;
	else
		w = -LIB_SQRT(t);
}

// --------------------------------------------------------------------------
// Rotate a point by quaternion.  v' = q.p.q*, where p = <0, v>.
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::Rotate(Vector3<T> &v) const
{
	Quaternion<T> qf = *this * v * ~(*this);
	v.x = qf.x;
	v.y = qf.y;
	v.z = qf.z;
}

// --------------------------------------------------------------------------
// Setup the quaternion to perform a rotation, given the angular displacement
// in matrix form.
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::FromMatrix(const Matrix4x4<T> &m)
{
	T trace = m.m11 + m.m22 + m.m33 + 1.0;
	
	if (trace > 0.0001)
	{
		T s = 0.5 / LIB_SQRT(trace);
		w = 0.25 / s;
		x = (m.m23 - m.m32) * s;
		y = (m.m31 - m.m13) * s;
		z = (m.m12 - m.m21) * s;
	}
	else
	{
		if ((m.m11 > m.m22) && (m.m11 > m.m33))
		{
			T s = 0.5 / LIB_SQRT(1.0 + m.m11 - m.m22 - m.m33);
			x = 0.25 / s;
			y = (m.m21 + m.m12) * s;
			z = (m.m31 + m.m13) * s;
			w = (m.m32 - m.m23) * s;
		}
		else if (m.m22 > m.m33)
		{
			T s = 0.5 / LIB_SQRT(1.0 + m.m22 - m.m11 - m.m33);
			x = (m.m21 + m.m12) * s;
			y = 0.25 / s;
			z = (m.m32 + m.m23) * s;
			w = (m.m31 - m.m13) * s;
		}
		else
		{
			T s = 0.5 / LIB_SQRT(1.0 + m.m33 - m.m11 - m.m22);
			x = (m.m31 + m.m13) * s;
			y = (m.m32 + m.m23) * s;
			z = 0.25 / s;
			w = (m.m21 - m.m12) * s;
		}
	}
}

// --------------------------------------------------------------------------
// Setup the quaternion to perform an object->inertial rotation, given the
// orientation in XYZ-Euler angles format.  x,y,z parameters must be in radians.
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::FromEulerAngles(T x, T y, T z)
{
	// Compute sine and cosine of the half angles
	T sr = LIB_SIN(x * 0.5);
	T cr = LIB_COS(x * 0.5);
	T sp = LIB_SIN(y * 0.5);
	T cp = LIB_COS(y * 0.5);
	T sy = LIB_SIN(z * 0.5);
	T cy = LIB_COS(z * 0.5);
	
	// Compute values
	w =  (cy * cp * cr) + (sy * sp * sr);
	x = -(sy * sp * cr) + (cy * cp * sr);
	y =  (cy * sp * cr) + (sy * cp * sr);
	z = -(cy * sp * sr) + (sy * cp * cr);
}

// --------------------------------------------------------------------------
// Setup the Euler angles, given an object->inertial rotation quaternion.
// Returned x,y,z are in radians.
// --------------------------------------------------------------------------
template <typename T>
inline void Quaternion<T>::ToEulerAngles(T &x, T &y, T &z) const
{
	// Compute Y-axis angle
	y = LIB_ASIN(2.0 * ((x * z) + (w * y)));
	
	// Compute cos and one over cos for optimization
	T cy = LIB_COS(y);
	T oneOverCosY = 1.0 / cy;
	
	if (LIB_FABS(cy) > 0.001)
	{
		// No gimball lock
		x = LIB_ATAN2(2.0 * ((w * x) - (y * z)) * oneOverCosY, (1.0 - 2.0 * (x*x + y*y)) * oneOverCosY);
		z = LIB_ATAN2(2.0 * ((w * z) - (x * y)) * oneOverCosY, (1.0 - 2.0 * (y*y + z*z)) * oneOverCosY);
	}
	else
	{
		// Gimbal lock case
		x = 0.0;
		z = LIB_ATAN2(2.0 * ((x * y) + (w * z)), 1.0 - 2.0 * (x*x + z*z));
	}
}

// --------------------------------------------------------------------------
// Return the rotation angle theta (in radians).
// --------------------------------------------------------------------------
template <typename T>
inline T
Quaternion<T>::RotationAngle() const
{
	// Compute the half angle.  Remember that w = cos(theta / 2)
	T thetaOver2 = SafeAcos(w);
	
	// Return the rotation angle
	return thetaOver2 * 2.0;
}

// --------------------------------------------------------------------------
// Return the rotation axis.
// --------------------------------------------------------------------------
template <typename T>
inline Vector3<T> Quaternion<T>::RotationAxis() const
{
	// Compute sin^2(theta/2).  Remember that w = cos(theta/2),
	// and sin^2(x) + cos^2(x) = 1
	T sinThetaOver2Sq = 1.0 - (w * w);
	
	// Protect against numerical imprecision
	if (sinThetaOver2Sq <= 0.0)
	{
		// Identity quaternion, or numerical imprecision.  Just
		// return any valid vector, since it doesn't matter
		
		return Vector3<T>(1.0, 0.0, 0.0);
	}
	
	// Compute 1 / sin(theta/2)
	T oneOverSinThetaOver2 = 1.0 / LIB_SQRT(sinThetaOver2Sq);
	
	// Return axis of rotation
	return Vector3<T>(
		x * oneOverSinThetaOver2,
		y * oneOverSinThetaOver2,
		z * oneOverSinThetaOver2
	);
}

// --------------------------------------------------------------------------
// Operator overloading for basic quaternion operations.
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> Quaternion<T>::operator+(const Quaternion<T> &q) const
{
	return Quaternion<T>(w + q.w, x + q.x, y + q.y, z + q.z);
}

template <typename T>
inline Quaternion<T> &Quaternion<T>::operator+=(const Quaternion<T> &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	
	return *this;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator-(const Quaternion<T> &q) const
{
	return Quaternion<T>(w - q.w, x - q.x, y - q.y, z - q.z);
}

template <typename T>
inline Quaternion<T> &Quaternion<T>::operator-=(const Quaternion<T> &q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	
	return *this;
}

// Quaternion multiplication.  The order of multiplication, from left
// to right, corresponds to the order of concatenated rotations.
// NOTE: Quaternion multiplication is NOT commutative, p * q != q * p
template <typename T>
inline Quaternion<T> Quaternion<T>::operator*(const Quaternion<T> &q) const
{
	// We use the Grassman product formula:
	// pq = <w1w2 - dot(v1, v2), w1v2 + w2v1 + cross(v1, v2)>
	return Quaternion<T>(
		(w * q.w) - (x * q.x) - (y * q.y) - (z * q.z),
		(x * q.w) + (w * q.x) + (y * q.z) - (z * q.y),
		(y * q.w) + (w * q.y) + (z * q.x) - (x * q.z),
		(z * q.w) + (w * q.z) + (x * q.y) - (y * q.x)
	);
}

template <typename T>
inline Quaternion<T> &Quaternion<T>::operator*=(const Quaternion<T> &q)
{
	*this = *this * q;
	return *this;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator*(const Vector3<T> &v) const
{
	// q * v = q * p where p = <0,v>
	// Thus, we can simplify the operations.
	return Quaternion<T>(
		- (x * v.x) - (y * v.y) - (z * v.z),
		  (w * v.x) + (y * v.z) - (z * v.y),
		  (w * v.y) + (z * v.x) - (x * v.z),
		  (w * v.z) + (x * v.y) - (y * v.x)
	);
}

template <typename T>
inline Quaternion<T> &Quaternion<T>::operator*=(const Vector3<T> &v)
{
	*this = *this * v;
	return *this;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator*(T k) const
{
	return Quaternion<T>(w * k, x * k, y * k, z * k);
}

template <typename T>
inline Quaternion<T> &Quaternion<T>::operator*=(T k)
{
	w *= k;
	x *= k;
	y *= k;
	z *= k;
	
	return *this;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator/(T k) const
{
	T oneOverK = 1.0 / k;
	return Quaternion<T>(w * oneOverK, x * oneOverK, y * oneOverK, z * oneOverK);
}

template <typename T>
inline Quaternion<T> &Quaternion<T>::operator/=(T k)
{
	T oneOverK = 1.0 / k;
	w *= oneOverK;
	x *= oneOverK;
	y *= oneOverK;
	z *= oneOverK;
	
	return *this;
}

// Quaternion conjugate
template <typename T>
inline Quaternion<T> Quaternion<T>::operator~() const
{
	return Quaternion<T>(w, -x, -y, -z);
}

// Quaternion negation
template <typename T>
inline Quaternion<T> Quaternion<T>::operator-() const
{
	return Quaternion<T>(-w, -x, -y, -z);
}

// --------------------------------------------------------------------------
// Nonmember Quaternion functions
// --------------------------------------------------------------------------

// Scalar on left multiplication
template <typename T>
inline Quaternion<T> operator*(T k, const Quaternion<T> &q)
{
	return Quaternion<T>(q.w * k, q.x * k, q.y * k, q.z * k);
}

// Quaternion dot product
template <typename T>
inline T DotProduct(const Quaternion<T> &a, const Quaternion<T> &b)
{
	return ((a.w * b.w) + (a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

// Compute the quaternion conjugate.  This is the quaternian
// with the opposite rotation as the original quaternion.
template <typename T>
inline Quaternion<T> Conjugate(const Quaternion<T> &q)
{
	return Quaternion<T>(q.w, -q.x, -q.y, -q.z);
}

// Compute the inverse quaternion (for unit quaternion only).
template <typename T>
inline Quaternion<T>
Inverse (const Quaternion<T> &q)
{
	// Assume this is a unit quaternion! No check for this!
	Quaternion<T> res(q.w, -q.x, -q.y, -q.z);
	res.Normalize();
	return res;
}

// --------------------------------------------------------------------------
// Setup the quaternion to rotate about the specified axis.  theta must
// be in radians.
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> RotationQuaternion(Axis axis, T theta)
{
	Quaternion<T> res;
	
	// Compute the half angle
	T thetaOver2 = theta * 0.5;
	
	// Set the values
	switch (axis)
	{
		case AxisX:
			res.w = LIB_COS(thetaOver2);
			res.x = LIB_SIN(thetaOver2);
			res.y = 0.0;
			res.z = 0.0;
		break;
		
		case AxisY:
			res.w = LIB_COS(thetaOver2);
			res.x = 0.0;
			res.y = LIB_SIN(thetaOver2);
			res.z = 0.0;
		break;
		
		case AxisZ:
			res.w = LIB_COS(thetaOver2);
			res.x = 0.0;
			res.y = 0.0;
			res.z = LIB_SIN(thetaOver2);
		break;
		
		default:
			// Bad axis
			ASSERT(false);
	}
	
	return res;
}

template <typename T>
inline Quaternion<T> RotationQuaternion(const Vector3<T> &axis, T theta)
{
	Quaternion<T> res;
	
	// The axis of rotation must be normalized
	ASSERT(LIB_FABS(DotProduct(axis, axis) - 1.0) < 0.001);
	
	// Compute the half angle and its sin
	T thetaOver2 = theta * 0.5;
	T sinThetaOver2 = LIB_SIN(thetaOver2);
	
	// Set the values
	res.w = LIB_COS(thetaOver2);
	res.x = axis.x * sinThetaOver2;
	res.y = axis.y * sinThetaOver2;
	res.z = axis.z * sinThetaOver2;
	
	return res;
}

// --------------------------------------------------------------------------
// Unit quaternion logarithm. log(q) = log(cos(theta) + n*sin(theta))
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> Log(const Quaternion<T> &q)
{
	Quaternion<T> res;
	res.w = 0.0;
	
	if (LIB_FABS(q.w) < 1.0)
	{
		T theta = LIB_ACOS(q.w);
		T sin_theta = LIB_SIN(theta);
		
		if (LIB_FABS(sin_theta) > 0.00001)
		{
			T thetaOverSinTheta = theta / sin_theta;
			res.x = q.x * thetaOverSinTheta;
			res.y = q.y * thetaOverSinTheta;
			res.z = q.z * thetaOverSinTheta;
			
			return res;
		}
	}
	
	res.x = q.x;
	res.y = q.y;
	res.z = q.z;
	
	return res;
}

// --------------------------------------------------------------------------
// Quaternion exponential.
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> Exp(const Quaternion<T> &q)
{
	T theta = LIB_SQRT(DotProduct(q, q));
	T sin_theta = LIB_SIN(theta);
	
	Quaternion<T> res;
	res.w = LIB_COS(theta);
	
	if (LIB_FABS(sin_theta) > 0.00001)
	{
		T sinThetaOverTheta = sin_theta / theta;
		res.x = q.x * sinThetaOverTheta;
		res.y = q.y * sinThetaOverTheta;
		res.z = q.z * sinThetaOverTheta;
	}
	else
	{
		res.x = q.x;
		res.y = q.y;
		res.z = q.z;
	}
	
	return res;
}

// --------------------------------------------------------------------------
// Quaternion exponentiation.
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> Pow(const Quaternion<T> &q, T exponent)
{
	// Check for the case of an identity quaternion.
	// This will protect against divide by zero
	if (LIB_FABS(q.w) > 0.9999)
		return q;
	
	// Extract the half angle alpha (alpha = theta/2)
	T alpha = LIB_ACOS(q.w);
	
	// Compute new alpha value
	T newAlpha = alpha * exponent;
	
	// Compute new quaternion
	Vector3<T> n(q.x, q.y, q.z);
	n *= LIB_SIN(newAlpha) / LIB_SIN(alpha);
	
	return Quaternion<T>(LIB_COS(newAlpha), n);
}

// --------------------------------------------------------------------------
// Spherical linear interpolation.
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> Slerp(const Quaternion<T> &q0, const Quaternion<T> &q1, T t)
{
	// Check for out-of range parameter and return edge points if so
	if (t <= 0.0) return q0;
	if (t >= 1.0) return q1;
	
	// Compute "cosine of angle between quaternions" using dot product
	T cosOmega = DotProduct(q0, q1);
	
	// If negative dot, use -q1.  Two quaternions q and -q
	// represent the same rotation, but may produce
	// different slerp.  We chose q or -q to rotate using
	// the acute angle.
	T q1w = q1.w;
	T q1x = q1.x;
	T q1y = q1.y;
	T q1z = q1.z;
	
	if (cosOmega < 0.0)
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}
	
	// We should have two unit quaternions, so dot should be <= 1.0
	ASSERT(cosOmega < 1.1);
	
	// Compute interpolation fraction, checking for quaternions
	// almost exactly the same
	T k0, k1;
	
	if (cosOmega > 0.9999)
	{
		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero
		
		k0 = 1.0 - t;
		k1 = t;
	}
	else
	{
		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1
		T sinOmega = LIB_SQRT(1.0 - (cosOmega * cosOmega));
		
		// Compute the angle from its sin and cosine
		T omega = LIB_ATAN2(sinOmega, cosOmega);
		
		// Compute inverse of denominator, so we only have
		// to divide once
		T oneOverSinOmega = 1.0 / sinOmega;
		
		// Compute interpolation parameters
		k0 = LIB_SIN((1.0 - t) * omega) * oneOverSinOmega;
		k1 = LIB_SIN(t * omega) * oneOverSinOmega;
	}
	
	// Interpolate and return new quaternion
	return Quaternion<T>(
		(k0 * q0.w) + (k1 * q1w),
		(k0 * q0.x) + (k1 * q1x),
		(k0 * q0.y) + (k1 * q1y),
		(k0 * q0.z) + (k1 * q1z)
	);
}

// --------------------------------------------------------------------------
// Spherical cubic interpolation.
// squad = slerp (slerp (q0, q1, t), slerp (qa, qb, t), 2t(1 - t)).
// --------------------------------------------------------------------------
template <typename T>
inline Quaternion<T> Squad(const Quaternion<T> &q0, const Quaternion<T> &qa, const Quaternion<T> &qb, const Quaternion<T> &q1, T t)
{
	T slerp_t = 2.0 * t * (1.0 - t);
	
	Quaternion<T> slerpq0 = Slerp(q0, q1, t);
	Quaternion<T> slerpq1 = Slerp(qa, qb, t);
	
	return Slerp(slerpq0, slerpq1, slerp_t);
}

// --------------------------------------------------------------------------
// Compute intermediate quaternions for building spline segments.
// --------------------------------------------------------------------------
template <typename T>
inline void Intermediate(const Quaternion<T> &qprev, const Quaternion<T> &qcurr, const Quaternion<T> &qnext, Quaternion<T> &qa, Quaternion<T> &qb)
{
	// We should have unit quaternions
	ASSERT(DotProduct(qprev, qprev) <= 1.0001);
	ASSERT(DotProduct(qcurr, qcurr) <= 1.0001);
	
	Quaternion<T> inv_prev = Conjugate(qprev);
	Quaternion<T> inv_curr = Conjugate(qcurr);
	
	Quaternion<T> p0 = inv_prev * qcurr;
	Quaternion<T> p1 = inv_curr * qnext;
	
	Quaternion<T> arg = (Log(p0) - Log(p1)) * 0.25;
	
	qa = qcurr * Exp( arg);
	qb = qcurr * Exp(-arg);
}

*/
#endif // __QUATERNION_H__
