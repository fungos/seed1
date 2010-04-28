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

/*! \file Matrix4x4.h
	\author	Thiago C. Moraes
			Danny Angelo Carminati Grein
	\brief Matrix
*/

/////////////////////////////////////////////////////////////////////////////
//
// class Matrix4x4<T> implementation.
//
// --------------------------------------------------------------------------
//
// MATRIX ORGANIZATION
//
// The purpose of this class is so that a user might perform transformations
// without fiddling with plus or minus signs or transposing the matrix
// until the output "looks right".  But of course, the specifics of the
// internal representation is important.  Not only for the implementation
// in this file to be correct, but occasionally direct access to the
// matrix variables is necessary, or beneficial for optimization.  Thus,
// we document our matrix conventions here.
//
// Strict adherance to linear algebra rules dictates that the
// multiplication of a 4x4 matrix by a 3D vector is actually undefined.
// To circumvent this, we can consider the input and output vectors as
// having an assumed fourth coordinate of 1.  Also, since the rightmost
// column is [ 0 0 0 1 ], we can simplificate calculations ignoring
// this last column. This is shown below:
//
//         | m11 m12 m13 0 | | x |   | x'|
//         | m21 m22 m23 0 | | y | = | y'|
//         | m31 m32 m33 0 | | z |   | z'|
//         | tx  ty  tz  1 | | 1 |   | 1 |
//
// We use row vectors to represent the right, up and forward vectors
// in the 4x4 matrix.  OpenGL uses column vectors, but the elements of
// an OpenGL matrix are ordered in columns so that m[i][j] is in row j
// and column i.  This is the reverse of the standard C convention in
// which m[i][j] is in row i and column j.  The matrix should be
// transposed before being sent to OpenGL.
//
//   | m11 m21 m31 tx |    |  m0  m4  m8 m12 |    |  m0  m1  m2  m3 |
//   | m12 m22 m32 ty |    |  m1  m5  m9 m13 |    |  m4  m5  m6  m7 |
//   | m13 m23 m33 tz |    |  m2  m6 m10 m14 |    |  m8  m9 m10 m11 |
//   |  0   0   0  tw |    |  m3  m7 m11 m15 |    | m12 m13 m14 m15 |
//
//      OpenGL style          OpenGL matrix            standard C
//                             arrangement             convention
//
// Fortunately, accessing to the raw matrix data via the m[] array gives
// us the transpose matrix; i.e. in OpenGL form, so that we can directly use
// it with glLoadMatrix{fd}() or glMultMatrix{fd}().
//
// Also, since the rightmost column (in standard C form) should always
// be [ 0 0 0 1 ], and sice these values (h14, h24, h34 and tw) are
// initialized in constructors, we don't need to modify them in our
// matrix operations, so we don't perform useless calculations...
//
// The right-hand rule is used to define "positive" rotation.
//
//               +y                          +x'
//                |                           |
//                |                           |
//                |______ +x        +y' ______|
//               /                            /
//              /                            /
//             +z                          +z'
//
//          initial position      Positive rotation of
//                                 pi/2 around z-axis
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "MathUtil.h"

namespace Seed {

// Type definitions
enum eAxis
{
	AxisX,
	AxisY,
	AxisZ
};

template <typename T> class Matrix4x4
{
	public:
		// Constructor - Initialize the last (never used) row of the matrix
		// so that we can do any operation on matrices on the 3x4 portion
		// and forget that line which will (and should) never change.
		Matrix4x4()
			: h14(0.0f)
			, h24(0.0f)
			, h34(0.0f)
			, tw(1.0f)
		{
		}
		
		~Matrix4x4()
		{
		}

		// Note that we don't define the copy constructor and let the compiler
		// doing it itself because such initialization is not necessary
		// since the source matrix has its last row already initialized...
		void Identity();
		void Transpose();
		void Invert();
		void SetTranslation(const Vector3<T> &v);

		void Transform(Vector3<T> &v) const;
		void Rotate(Vector3<T> &v) const;
		void InverseRotate(Vector3<T> &v) const;
		void InverseTranslate(Vector3<T> &v) const;

		void FromQuaternion(const Quaternion<T> &q);

		// Matrix <-> Euler conversions; XYZ rotation order; angles in radians
		void FromEulerAngles(T x, T y, T z);
		void ToEulerAngles(T &x, T &y, T &z) const;
		
		// Return a base vector from the matrix
		Vector3<T> RightVector() const;
		Vector3<T> UpVector() const;
		Vector3<T> ForwardVector() const;
		Vector3<T> TranslationVector() const;
		
		// Accessor.  This allows to use the matrix object like an array of T. For example:
		// Matrix4x4<float> mat;
		// float f = mat[4]; // access to m21
		operator const T*()
		{
			return m;
		}
		
		// The values of the matrix.  Basically the upper 3x3 portion
		// contains a linear transformation, and the last column is the
		// translation portion. Here data is transposed, see the Mathlib.inl
		// for more details.
		union
		{
			struct
			{
				T m11, m12, m13, h14;
				T m21, m22, m23, h24;
				T m31, m32, m33, h34;
				T tx,  ty,  tz,  tw;
			};
			
			// Access to raw packed matrix data (usefull for glLoadMatrixf() and glMultMatrixf())
			T m[16];
		};
};

//
// Nonmember Matrix4x4 functions
//

// Matrix multiplication
template <typename T> Matrix4x4<T> operator*(const Matrix4x4<T> &a, const Matrix4x4<T> &b);
template <typename T> Matrix4x4<T> &operator*=(Matrix4x4<T> &a, const Matrix4x4<T> &b);

// Vector transformation
template <typename T> Vector3<T> operator*(const Matrix4x4<T> &m, const Vector3<T> &p);

// Transpose matrix
template <typename T> Matrix4x4<T> Transpose(const Matrix4x4<T> &m);

// Invert matrix
template <typename T> Matrix4x4<T> Invert(const Matrix4x4<T> &m);

//
// Matrix-builder functions
//
template <typename T> Matrix4x4<T> RotationMatrix(eAxis axis, T theta);
template <typename T> Matrix4x4<T> RotationMatrix(const Vector3<T> &axis, T theta);
template <typename T> Matrix4x4<T> TranslationMatrix(T x, T y, T z);
template <typename T> Matrix4x4<T> TranslationMatrix(const Vector3<T> &v);
template <typename T> Matrix4x4<T> ScaleMatrix(const Vector3<T> &s);
template <typename T> Matrix4x4<T> ScaleAlongAxisMatrix(const Vector3<T> &axis, T k);
template <typename T> Matrix4x4<T> ShearMatrix(eAxis axis, T s, T t);
template <typename T> Matrix4x4<T> ProjectionMatrix(const Vector3<T> &n);
template <typename T> Matrix4x4<T> ReflectionMatrix(eAxis axis, T k);
template <typename T> Matrix4x4<T> AxisReflectionMatrix(const Vector3<T> &n);

template <typename T> Matrix4x4<T> LookAtMatrix(const Vector3<T> &camPos, const Vector3<T> &target, const Vector3<T> &camUp);
template <typename T> Matrix4x4<T> FrustumMatrix(T l, T r, T b, T t, T n, T f);
template <typename T> Matrix4x4<T> PerspectiveMatrix(T fovY, T aspect, T n, T f);
template <typename T> Matrix4x4<T> OrthoMatrix(T l, T r, T b, T t, T n, T f);
template <typename T> Matrix4x4<T> OrthoNormalMatrix(const Vector3<T> &xdir, const Vector3<T> &ydir, const Vector3<T> &zdir);

// Predefined Matrix4x4 types
typedef Matrix4x4<f32> Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;

// --------------------------------------------------------------------------
// Set matrix to identity.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::Identity()
{
	m11 = 1.0; m21 = 0.0; m31 = 0.0; tx = 0.0;
	m12 = 0.0; m22 = 1.0; m32 = 0.0; ty = 0.0;
	m13 = 0.0; m23 = 0.0; m33 = 1.0; tz = 0.0;
	h14 = 0.0; h24 = 0.0; h34 = 0.0; tw = 1.0;
}

// --------------------------------------------------------------------------
// Transpose the current matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::Transpose()
{
	*this = Transpose(*this);
}

// --------------------------------------------------------------------------
// Invert the current matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::Invert()
{
	*this = Invert(*this);
}

// --------------------------------------------------------------------------
// Set the translation portion of the matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::SetTranslation(const Vector3<T> &v)
{
	tx = v.x;
	ty = v.y;
	tz = v.z;
}

// --------------------------------------------------------------------------
// Transform a point by the matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::Transform(Vector3<T> &v) const
{
	// Grind through the linear algebra.
	v = Vector3<T>(
		(v.x * m11) + (v.y * m21) + (v.z * m31) + tx,
		(v.x * m12) + (v.y * m22) + (v.z * m32) + ty,
		(v.x * m13) + (v.y * m23) + (v.z * m33) + tz
	);
}

// --------------------------------------------------------------------------
// Rotate a point by the 3x3 upper left portion of the matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::Rotate(Vector3<T> &v) const
{
	v = Vector3<T>(
		(v.x * m11) + (v.y * m21) + (v.z * m31),
		(v.x * m12) + (v.y * m22) + (v.z * m32),
		(v.x * m13) + (v.y * m23) + (v.z * m33)
	);
}

// --------------------------------------------------------------------------
// Rotate a point by the inverse 3x3 upper left portion of the matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::InverseRotate(Vector3<T> &v) const
{
	v = Vector3<T>(
		(v.x * m11) + (v.y * m12) + (v.z * m13),
		(v.x * m21) + (v.y * m22) + (v.z * m23),
		(v.x * m31) + (v.y * m32) + (v.z * m33)
	);
}

// --------------------------------------------------------------------------
// Translate a point by the inverse matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::InverseTranslate(Vector3<T> &v) const
{
	v.x -= tx;
	v.y -= ty;
	v.z -= tz;
}

// --------------------------------------------------------------------------
// Convert a quaternion to a matrix.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::FromQuaternion(const Quaternion<T> &q)
{
	// Compute a few values to optimize common subexpressions
	T ww = 2.0 * q.w;
	T xx = 2.0 * q.x;
	T yy = 2.0 * q.y;
	T zz = 2.0 * q.z;
	
	// Set the matrix elements.  There is still a little more
	// opportunity for optimization due to the many common
	// subexpressions.  We'll let the compiler handle that...
	m11 = 1.0 - (yy * q.y) - (zz * q.z);
	m12 = (xx * q.y) + (ww * q.z);
	m13 = (xx * q.z) - (ww * q.y);
	
	m21 = (xx * q.y) - (ww * q.z);
	m22 = 1.0 - (xx * q.x) - (zz * q.z);
	m23 = (yy * q.z) + (ww * q.x);
	
	m31 = (xx * q.z) + (ww * q.y);
	m32 = (yy * q.z) - (ww * q.x);
	m33 = 1.0 - (xx * q.x) - (yy * q.y);
	
	// Reset the translation portion
	tx = ty = tz = 0.0;
}

// --------------------------------------------------------------------------
// Setup a rotation matrix, given three X-Y-Z rotation angles. The
// rotations are performed first on x-axis, then y-axis and finaly z-axis.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::FromEulerAngles(T x, T y, T z)
{
	// Fetch sine and cosine of angles
	T cx = LIB_COS(x);
	T sx = LIB_SIN(x);
	T cy = LIB_COS(y);
	T sy = LIB_SIN(y);
	T cz = LIB_COS(z);
	T sz = LIB_SIN(z);
	
	T sxsy = sx * sy;
	T cxsy = cx * sy;
	
	// Fill in the matrix elements
	m11 =  (cy * cz);
	m12 =  (sxsy * cz) + (cx * sz);
	m13 = -(cxsy * cz) + (sx * sz);
	
	m21 = -(cy * sz);
	m22 = -(sxsy * sz) + (cx * cz);
	m23 =  (cxsy * sz) + (sx * cz);
	
	m31 =  (sy);
	m32 = -(sx * cy);
	m33 =  (cx * cy);
	
	// Reset the translation portion
	tx = ty = tz = 0.0;
}

// --------------------------------------------------------------------------
// Setup the euler angles in radians, given a rotation matrix. The rotation
// matrix could have been obtained from euler angles given the expression:
//   M = X.Y.Z
// where X, Y and Z are rotation matrices about X, Y and Z axes.
// This is the "opposite" of the fromEulerAngles function.
// --------------------------------------------------------------------------
template <typename T>
inline void Matrix4x4<T>::ToEulerAngles(T &x, T &y, T &z) const
{
	// Compute Y-axis angle
	y = LIB_ASIN(m31);
	
	// Compute cos and one over cos for optimization
	T cy = LIB_COS(y);
	T oneOverCosY = 1.0 / cy;
	
	if (LIB_FABS(cy) > 0.001)
	{
		// No gimball lock
		x = LIB_ATAN2(-m32 * oneOverCosY, m33 * oneOverCosY);
		z = LIB_ATAN2(-m21 * oneOverCosY, m11 * oneOverCosY);
	}
	else
	{
		// Gimbal lock case
		x = 0.0;
		z = LIB_ATAN2(m12, m22);
	}
}

// --------------------------------------------------------------------------
// Return a base vector from the matrix.
// --------------------------------------------------------------------------
template <typename T>
inline Vector3<T> Matrix4x4<T>::RightVector() const
{
	return Vector3<T> (m11, m12, m13);
}

template <typename T>
inline Vector3<T> Matrix4x4<T>::UpVector() const
{
	return Vector3<T>(m21, m22, m23);
}

template <typename T>
inline Vector3<T> Matrix4x4<T>::ForwardVector() const
{
	return Vector3<T>(m31, m32, m33);
}

template <typename T>
inline Vector3<T> Matrix4x4<T>::TranslationVector() const
{
	return Vector3<T>(tx, ty, tz);
}

// --------------------------------------------------------------------------
// Nonmember Matrix4x4<T> functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Matrix concatenation.
// We also provide a *= operator, as per C convention.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> operator*(const Matrix4x4<T> &a, const Matrix4x4<T> &b)
{
	Matrix4x4<T> res;
	
	// Compute the left 4x3 (linear transformation) portion
	res.m11 = (a.m11 * b.m11) + (a.m21 * b.m12) + (a.m31 * b.m13);
	res.m12 = (a.m12 * b.m11) + (a.m22 * b.m12) + (a.m32 * b.m13);
	res.m13 = (a.m13 * b.m11) + (a.m23 * b.m12) + (a.m33 * b.m13);
	
	res.m21 = (a.m11 * b.m21) + (a.m21 * b.m22) + (a.m31 * b.m23);
	res.m22 = (a.m12 * b.m21) + (a.m22 * b.m22) + (a.m32 * b.m23);
	res.m23 = (a.m13 * b.m21) + (a.m23 * b.m22) + (a.m33 * b.m23);
	
	res.m31 = (a.m11 * b.m31) + (a.m21 * b.m32) + (a.m31 * b.m33);
	res.m32 = (a.m12 * b.m31) + (a.m22 * b.m32) + (a.m32 * b.m33);
	res.m33 = (a.m13 * b.m31) + (a.m23 * b.m32) + (a.m33 * b.m33);
	
	// Compute the translation portion
	res.tx = (a.m11 * b.tx) + (a.m21 * b.ty) + (a.m31 * b.tz) + a.tx;
	res.ty = (a.m12 * b.tx) + (a.m22 * b.ty) + (a.m32 * b.tz) + a.ty;
	res.tz = (a.m13 * b.tx) + (a.m23 * b.ty) + (a.m33 * b.tz) + a.tz;
	
	return res;
}

template <typename T>
inline Matrix4x4<T> &operator*=(Matrix4x4<T> &a, const Matrix4x4<T> &b)
{
	a = a * b;
	return a;
}

// --------------------------------------------------------------------------
// Transform a point by a matrix.  This makes using the vector class look
// like it does with linear algebra notation on paper.
// --------------------------------------------------------------------------
template <typename T>
inline Vector3<T> operator*(const Matrix4x4<T> &m, const Vector3<T> &p)
{
	Vector3<T> res(p);
	m.Transform(res);
	return res;
}

// --------------------------------------------------------------------------
// Return the transpose matrix.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> Transpose(const Matrix4x4<T> &m)
{
	Matrix4x4<T> res;
	
	res.m11 = m.m11; res.m21 = m.m12; res.m31 = m.m13; res.tx = m.h14;
	res.m12 = m.m21; res.m22 = m.m22; res.m32 = m.m23; res.ty = m.h24;
	res.m13 = m.m31; res.m23 = m.m32; res.m33 = m.m33; res.tz = m.h34;
	res.h14 = m.tx;  res.h24 = m.ty;  res.h34 = m.tz;  res.tw = m.tw;
	
	return res;
}

// --------------------------------------------------------------------------
// Compute the determinant of the 3x3 portion of the matrix.
// --------------------------------------------------------------------------
template <typename T>
inline static T Determinant3x3(const Matrix4x4<T> &m)
{
	return m.m11 * ((m.m22 * m.m33) - (m.m23 * m.m32)) 
		 + m.m12 * ((m.m23 * m.m31) - (m.m21 * m.m33))
		 + m.m13 * ((m.m21 * m.m32) - (m.m22 * m.m31));
}

// --------------------------------------------------------------------------
// Compute the inverse of a matrix.  We use the classical adjoint divided
// by the determinant method.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> Invert(const Matrix4x4<T> &m)
{
	// Compute the determinant of the 3x3 portion
	T det = Determinant3x3(m);
	
	// If we're singular, then the determinant is zero and there's no inverse
	ASSERT(LIB_FABS(det) > 0.000001);
	
	// Compute one over the determinant, so we divide once and
	// can *multiply* per element
	T oneOverDet = 1.0 / det;
	
	// Compute the 3x3 portion of the inverse, by
	// dividing the adjoint by the determinant
	Matrix4x4<T> res;
	
	res.m11 = ((m.m22 * m.m33) - (m.m23 * m.m32)) * oneOverDet;
	res.m12 = ((m.m13 * m.m32) - (m.m12 * m.m33)) * oneOverDet;
	res.m13 = ((m.m12 * m.m23) - (m.m13 * m.m22)) * oneOverDet;
	
	res.m21 = ((m.m23 * m.m31) - (m.m21 * m.m33)) * oneOverDet;
	res.m22 = ((m.m11 * m.m33) - (m.m13 * m.m31)) * oneOverDet;
	res.m23 = ((m.m13 * m.m21) - (m.m11 * m.m23)) * oneOverDet;
	
	res.m31 = ((m.m21 * m.m32) - (m.m22 * m.m31)) * oneOverDet;
	res.m32 = ((m.m12 * m.m31) - (m.m11 * m.m32)) * oneOverDet;
	res.m33 = ((m.m11 * m.m22) - (m.m12 * m.m21)) * oneOverDet;
	
	// Compute the translation portion of the inverse
	res.tx = -((m.tx * res.m11) + (m.ty * res.m21) + (m.tz * res.m31));
	res.ty = -((m.tx * res.m12) + (m.ty * res.m22) + (m.tz * res.m32));
	res.tz = -((m.tx * res.m13) + (m.ty * res.m23) + (m.tz * res.m33));
	
	// Return it.
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform a rotation about one of the three cardinal
// X-Y-Z axes.
//
// The axis of rotation is specified by the 1-based "axis" index.
//
// theta is the amount of rotation, in radians.  The right-hand rule is
// used to define "positive" rotation.
//
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> RotationMatrix(eAxis axis, T theta)
{
	Matrix4x4<T> res;
	
	// Get sin and cosine of rotation angle
	T s = LIB_SIN(theta);
	T c = LIB_COS(theta);
	
	// Check which axis they are rotating about
	switch (axis)
	{
		case AxisX: // Rotate about the x-axis
			res.m11 = 1.0; res.m21 = 0.0; res.m31 = 0.0;
			res.m12 = 0.0; res.m22 = c;   res.m32 = -s;
			res.m13 = 0.0; res.m23 = s;   res.m33 =  c;
		break;
		
		case AxisY: // Rotate about the y-axis
			res.m11 = c;   res.m21 = 0.0; res.m31 = s;
			res.m12 = 0.0; res.m22 = 1.0; res.m32 = 0.0;
			res.m13 = -s;  res.m23 = 0.0; res.m33 = c;
		break;
		
		case AxisZ: // Rotate about the z-axis
			res.m11 = c;   res.m21 = -s;  res.m31 = 0.0;
			res.m12 = s;   res.m22 =  c;  res.m32 = 0.0;
			res.m13 = 0.0; res.m23 = 0.0; res.m33 = 1.0;
		break;
		
		default:
			// bogus axis index
			ASSERT(false);
	}
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}

//---------------------------------------------------------------------------
// Setup the matrix to perform a rotation about an arbitrary axis.
// The axis of rotation must pass through the origin.
//
// axis defines the axis of rotation, and must be a unit vector.
//
// theta is the amount of rotation, in radians.  The right-hand rule is
// used to define "positive" rotation.
//
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> RotationMatrix(const Vector3<T> &axis, T theta)
{
	Matrix4x4<T> res;
	
	// Quick sanity check to make sure they passed in a unit vector to specify the axis
	ASSERT(LIB_FABS(DotProduct(axis, axis) - 1.0) < 0.001);
	
	// Get sin and cosine of rotation angle
	T s = LIB_SIN(theta);
	T c = LIB_COS(theta);
	
	// Compute 1 - cos(theta) and some common subexpressions
	T a = 1.0 - c;
	T ax = a * axis.x;
	T ay = a * axis.y;
	T az = a * axis.z;
	
	// Set the matrix elements.  There is still a little more
	// opportunity for optimization due to the many common
	// subexpressions.  We'll let the compiler handle that...
	res.m11 = (ax * axis.x) + c;
	res.m12 = (ax * axis.y) + (axis.z * s);
	res.m13 = (ax * axis.z) - (axis.y * s);
	
	res.m21 = (ay * axis.x) - (axis.z * s);
	res.m22 = (ay * axis.y) + c;
	res.m23 = (ay * axis.z) + (axis.x * s);
	
	res.m31 = (az * axis.x) + (axis.y * s);
	res.m32 = (az * axis.y) - (axis.x * s);
	res.m33 = (az * axis.z) + c;
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// TranslationMatrix
//
// Build a translation matrix given a translation vector.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> TranslationMatrix(T x, T y, T z)
{
	return TranslationMatrix(Vector3<T>(x, y, z));
}

template <typename T>
inline Matrix4x4<T> TranslationMatrix(const Vector3<T> &v)
{
	Matrix4x4<T> res;
	
	res.m11 = 1.0; res.m21 = 0.0; res.m31 = 0.0; res.tx = v.x;
	res.m12 = 0.0; res.m22 = 1.0; res.m32 = 0.0; res.ty = v.y;
	res.m13 = 0.0; res.m23 = 0.0; res.m33 = 1.0; res.tz = v.z;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform scale on each axis.  For uniform scale by k,
// use a vector of the form Vector3( k, k, k ).
//
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> ScaleMatrix(const Vector3<T> &s)
{
	Matrix4x4<T> res;
	
	// Set the matrix elements.  Pretty straightforward
	res.m11 = s.x; res.m21 = 0.0; res.m31 = 0.0;
	res.m12 = 0.0; res.m22 = s.y; res.m32 = 0.0;
	res.m13 = 0.0; res.m23 = 0.0; res.m33 = s.z;
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform scale on each axis.
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> ScaleMatrix(T x, T y, T z)
{
	Matrix4x4<T> res;
	
	// Set the matrix elements.  Pretty straightforward
	res.m11 = x;   res.m21 = 0.0; res.m31 = 0.0;
	res.m12 = 0.0; res.m22 = y;   res.m32 = 0.0;
	res.m13 = 0.0; res.m23 = 0.0; res.m33 = z;
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}


// --------------------------------------------------------------------------
// Setup the matrix to perform scale along an arbitrary axis.
//
// The axis is specified using a unit vector.
//
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> ScaleAlongAxisMatrix(const Vector3<T> &axis, T k)
{
	Matrix4x4<T> res;
	
	// Quick sanity check to make sure they passed in a unit vector to specify the axis
	ASSERT(LIB_FABS(DotProduct(axis, axis) - 1.0) < 0.001);
	
	// Compute k-1 and some common subexpressions
	T a = k - 1.0;
	T ax = a * axis.x;
	T ay = a * axis.y;
	T az = a * axis.z;
	
	// Fill in the matrix elements.  We'll do the common
	// subexpression optimization ourselves here, since diagonally
	// opposite matrix elements are equal
	res.m11 = (ax * axis.x) + 1.0;
	res.m22 = (ay * axis.y) + 1.0;
	res.m32 = (az * axis.z) + 1.0;
	
	res.m12 = res.m21 = (ax * axis.y);
	res.m13 = res.m31 = (ax * axis.z);
	res.m23 = res.m32 = (ay * axis.z);
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform a shear
//
// The type of shear is specified by the 1-based "axis" index.  The effect
// of transforming a point by the matrix is described by the pseudocode
// below:
//
//	xAxis  =>  y += s * x, z += t * x
//	yAxis  =>  x += s * y, z += t * y
//	zAxis  =>  x += s * z, y += t * z
//
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> ShearMatrix(eAxis axis, T s, T t)
{
	Matrix4x4<T> res;

	// Check which type of shear they want
	switch (axis)
	{
		case AxisX: // Shear y and z using x
			res.m11 = 1.0; res.m21 = 0.0; res.m31 = 0.0;
			res.m12 = s;   res.m22 = 1.0; res.m32 = 0.0;
			res.m13 = t;   res.m23 = 0.0; res.m33 = 1.0;
		break;
		
		case AxisY: // Shear x and z using y
			res.m11 = 1.0; res.m21 = s;   res.m31 = 0.0;
			res.m12 = 0.0; res.m22 = 1.0; res.m32 = 0.0;
			res.m13 = 0.0; res.m23 = t;   res.m33 = 1.0;
		break;
		
		case AxisZ: // Shear x and y using z
			res.m11 = 1.0; res.m21 = 0.0; res.m31 = s;
			res.m12 = 0.0; res.m22 = 1.0; res.m32 = t;
			res.m13 = 0.0; res.m23 = 0.0; res.m33 = 1.0;
		break;
		
		default:
			// bogus axis index
			ASSERT(false);
	}
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform a projection onto a plane passing
// through the origin.  The plane is perpendicular to the
// unit vector n.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> ProjectionMatrix(const Vector3<T> &n)
{
	Matrix4x4<T> res;
	
	// Quick sanity check to make sure they passed in a unit vector
	// to specify the axis
	ASSERT(LIB_FABS(DotProduct(n, n) - 1.0) < 0.001);
	
	// Fill in the matrix elements.  We'll do the common
	// subexpression optimization ourselves here, since diagonally
	// opposite matrix elements are equal
	res.m11 = 1.0 - (n.x * n.x);
	res.m22 = 1.0 - (n.y * n.y);
	res.m33 = 1.0 - (n.z * n.z);
	
	res.m12 = res.m21 = -(n.x * n.y);
	res.m13 = res.m31 = -(n.x * n.z);
	res.m23 = res.m32 = -(n.y * n.z);
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform a reflection about a plane parallel
// to a cardinal plane.
//
// axis is a 1-based index which specifies the plane to project about:
//
//	xAxis => reflect about the plane x=k
//	yAxis => reflect about the plane y=k
//	zAxis => reflect about the plane z=k
//
// The translation is set appropriately, since translation must occur if
// k != 0
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> ReflectionMatrix(eAxis axis, T k)
{
	Matrix4x4<T> res;
	
	// Check which plane they want to reflect about
	switch (axis)
	{
		case AxisX: // Reflect about the plane x=k
			res.m11 = -1.0; res.m21 =  0.0; res.m31 =  0.0; res.tx = 2.0 * k;
			res.m12 =  0.0; res.m22 =  1.0; res.m32 =  0.0; res.ty = 0.0;
			res.m13 =  0.0; res.m23 =  0.0; res.m33 =  1.0; res.tz = 0.0;
		break;
		
		case AxisY: // Reflect about the plane y=k
			res.m11 =  1.0; res.m21 =  0.0; res.m31 =  0.0; res.tx = 0.0;
			res.m12 =  0.0; res.m22 = -1.0; res.m32 =  0.0; res.ty = 2.0 * k;
			res.m13 =  0.0; res.m23 =  0.0; res.m33 =  1.0; res.tz = 0.0;
		break;
		
		case AxisZ: // Reflect about the plane z=k
			res.m11 =  1.0; res.m21 =  0.0; res.m31 =  0.0; res.tx = 0.0;
			res.m12 =  0.0; res.m22 =  1.0; res.m32 =  0.0; res.ty = 0.0;
			res.m13 =  0.0; res.m23 =  0.0; res.m33 = -1.0; res.tz = 2.0 * k;
		break;
		
		default:
			// bogus axis index
			ASSERT(false);
	}
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform a reflection about an arbitrary plane
// through the origin.  The unit vector n is perpendicular to the plane.
//
// The translation portion is reset.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> AxisReflectionMatrix(const Vector3<T> &n)
{
	Matrix4x4<T> res;
	
	// Quick sanity check to make sure they passed in a unit vector
	// to specify the axis
	ASSERT(LIB_FABS(DotProduct(n, n) - 1.0) < 0.001);
	
	// Compute common subexpressions
	T ax = -2.0 * n.x;
	T ay = -2.0 * n.y;
	T az = -2.0 * n.z;
	
	// Fill in the matrix elements.  We'll do the common
	// subexpression optimization ourselves here, since diagonally
	// opposite matrix elements are equal
	res.m11 = 1.0 + (ax * n.x);
	res.m22 = 1.0 + (ay * n.y);
	res.m32 = 1.0 + (az * n.z);
	
	res.m12 = res.m21 = (ax * n.y);
	res.m13 = res.m31 = (ax * n.z);
	res.m23 = res.m32 = (ay * n.z);
	
	// Reset the translation portion
	res.tx = res.ty = res.tz = 0.00;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup the matrix to perform a "Look At" transformation like a first
// person camera.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> LookAtMatrix(const Vector3<T> &camPos, const Vector3<T> &target, const Vector3<T> &camUp)
{
	Matrix4x4<T> rot, trans;
	
	Vector3<T> forward(camPos - target);
	forward.Normalize();
	
	Vector3<T> right(CrossProduct(camUp, forward));
	Vector3<T> up(CrossProduct(forward, right));
	
	right.Normalize();
	up.Normalize();
	
	rot.m11 = right.x;
	rot.m21 = right.y;
	rot.m31 = right.z;
	
	rot.m12 = up.x;
	rot.m22 = up.y;
	rot.m32 = up.z;
	
	rot.m13 = forward.x;
	rot.m23 = forward.y;
	rot.m33 = forward.z;
	
	rot.tx  = 0.0;
	rot.ty  = 0.0;
	rot.tz  = 0.0;
	
	trans = TranslationMatrix(-camPos);
	
	return (rot * trans);
}

// --------------------------------------------------------------------------
// Setup a frustum matrix given the left, right, bottom, top, near, and far
// values for the frustum boundaries.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> FrustumMatrix(T l, T r, T b, T t, T n, T f)
{
	ASSERT(n >= 0.0);
	ASSERT(f >= 0.0);
	
	Matrix4x4<T> res;
	
	T width  = r - l;
	T height = t - b;
	T depth  = f - n;
	
	res.m[0] = (2 * n) / width;
	res.m[1] = 0.0;
	res.m[2] = 0.0;
	res.m[3] = 0.0;
	
	res.m[4] = 0.0;
	res.m[5] = (2 * n) / height;
	res.m[6] = 0.0;
	res.m[7] = 0.0;
	
	res.m[8] = (r + l) / width;
	res.m[9] = (t + b) / height;
	res.m[10]= -(f + n) / depth;
	res.m[11]= -1.0;
	
	res.m[12]= 0.0;
	res.m[13]= 0.0;
	res.m[14]= -(2 * f * n) / depth;
	res.m[15]= 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup a perspective matrix given the field-of-view in the Y direction
// in degrees, the aspect ratio of Y/X, and near and far plane distances.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> PerspectiveMatrix(T fovY, T aspect, T n, T f)
{
	Matrix4x4<T> res;
	
	T angle;
	T cot;
	
	angle = fovY / 2.0;
	angle = degToRad(angle);
	
	cot = LIB_COS(angle) / LIB_SIN(angle);
	
	res.m[0] = cot / aspect;
	res.m[1] = 0.0;
	res.m[2] = 0.0;
	res.m[3] = 0.0;
	
	res.m[4] = 0.0;
	res.m[5] = cot;
	res.m[6] = 0.0;
	res.m[7] = 0.0;
	
	res.m[8] = 0.0;
	res.m[9] = 0.0;
	res.m[10]= -(f + n) / (f - n);
	res.m[11]= -1.0;
	
	res.m[12]= 0.0;
	res.m[13]= 0.0;
	res.m[14]= -(2 * f * n) / (f - n);
	res.m[15]= 0.0;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup an orthographic Matrix4x4 given the left, right, bottom, top, near,
// and far values for the frustum boundaries.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> OrthoMatrix(T left, T right, T bottom, T top, T znear, T zfar)
{
	Matrix4x4<T> res;
	
	T width  = right - left;
	T height = top - bottom;
	T depth  = zfar - znear;
	
	res.m[0] =  2.0f / width;
	res.m[1] =  0.0f;
	res.m[2] =  0.0f;
	res.m[3] =  0.0f;
	
	res.m[4] =  0.0f;
	res.m[5] =  2.0f / height;
	res.m[6] =  0.0f;
	res.m[7] =  0.0f;
	
	res.m[8] =  0.0f;
	res.m[9] =  0.0f;
	res.m[10]= -2.0f / depth;
	res.m[11]=  0.0f;
	
	res.m[12]= -(right + left) / width;
	res.m[13]= -(top + bottom) / height;
	res.m[14]= -(zfar + znear) / depth;
	res.m[15]=  1.0f;
	
	return res;
}

// --------------------------------------------------------------------------
// Setup an orientation matrix using 3 basis normalized vectors.
// --------------------------------------------------------------------------
template <typename T>
inline Matrix4x4<T> OrthoNormalMatrix(const Vector3<T> &xdir, const Vector3<T> &ydir, const Vector3<T> &zdir)
{
	Matrix4x4<T> res;
	
	res.m[0] = xdir.x; res.m[4] = ydir.x; res.m[8] = zdir.x; res.m[12] = 0.0;
	res.m[1] = xdir.y; res.m[5] = ydir.y; res.m[9] = zdir.y; res.m[13] = 0.0;
	res.m[2] = xdir.z; res.m[6] = ydir.z; res.m[10]= zdir.z; res.m[14] = 0.0;
	res.m[3] = 0.0;    res.m[7] = 0.0;    res.m[11]= 0.0;    res.m[15] = 1.0;
	
	return res;
}

} // namespace

#endif // __MATRIX4X4_H__
