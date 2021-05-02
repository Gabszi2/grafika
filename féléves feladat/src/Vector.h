#ifndef INC_VECTOR_H_
#define INC_VECTOR_H_

#include "StdAfx.h"

class CVector2D
{
public:
	float x, y;

    inline CVector2D()
	{  }
    inline CVector2D(const float X, const float Y)
    { x = X; y = Y; }
    inline CVector2D(const float v[2])
    { x = v[0]; y = v[1]; }

	inline operator const float*() const
	{ return &x; }

    inline CVector2D operator+(const CVector2D &v) const
    { return CVector2D(x+v.x, y+v.y); }                

    inline CVector2D operator-(const CVector2D &v) const
    { return CVector2D(x-v.x, y-v.y); }                

	inline CVector2D& operator+=(const CVector2D &v)
	{ x += v.x; y += v.y; return *this; }

	inline CVector2D& operator-=(const CVector2D &v)
	{ x -= v.x; y -= v.y; return *this; }

	inline CVector2D& operator-()
	{ x = -x; y = -y; return *this; }
	
    inline CVector2D operator*(const float f) const
    { return CVector2D(x*f, y*f); }                

	inline CVector2D operator/(float f) const
	{ f = 1.0f/f; return CVector2D(x*f, y*f); }

	inline CVector2D& operator*=(const float f)
	{ x *= f; y *= f; return *this; }

	inline CVector2D& operator/=(float f)
	{ f = 1.0f/f; x *= f; y *= f; return *this; }

	inline float Dot(const CVector2D &v) const
    { return x*v.x + y*v.y; }

	inline float DistTo(const CVector2D &v) const
	{ return sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y)); }

	inline float Norm() const
	{ return sqrt(x*x + y*y); }
};

class CVector3D
{
public:
	float x, y, z;

    inline CVector3D()
	{  }
    inline CVector3D(const float X, const float Y, const float Z)
    { x = X; y = Y; z = Z; }
    inline CVector3D(const float v[3])
    { x = v[0]; y = v[1]; z = v[2]; }

	inline operator const float*() const
	{ return &x; }

	inline CVector3D operator+(const CVector3D &v) const
    { return CVector3D(x+v.x, y+v.y, z+v.z); }                

    inline CVector3D operator-(const CVector3D &v) const
    { return CVector3D(x-v.x, y-v.y, z-v.z); }                

	inline CVector3D& operator+=(const CVector3D &v)
	{ x += v.x; y += v.y; z += v.z; return *this; }

	inline CVector3D& operator-=(const CVector3D &v)
	{ x -= v.x; y -= v.y; z-= v.z; return *this; }

	inline CVector3D& operator-()
	{ x = -x; y = -y; z = -z; return *this; }
	
    inline CVector3D operator*(const float f) const
    { return CVector3D(x*f, y*f, z*f); }                

	inline CVector3D operator/(float f) const
	{ f = 1.0f/f; return CVector3D(x*f, y*f, z*f); }

	inline CVector3D& operator*=(const float f)
	{ x *= f; y *= f; z *= f; return *this; }

	inline CVector3D& operator/=(float f)
	{ f = 1.0f/f; x *= f; y *= f; z *= f; return *this; }

	inline float Dot(const CVector3D &v) const
    { return x*v.x + y*v.y + z*v.z; }

	inline float DistTo(const CVector3D &v) const
	{ return sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z)); }

	inline float Norm() const
	{ return sqrt(x*x + y*y + z*z); }
};

#endif

