#include <cmath>
#include "DreamVector.h"

namespace Core
{
    float DreamVector::Length() const
{
	float sq = x*x + y*y + z*z;
	return static_cast<float>(sqrt(sq));
}

//归一化
DreamVector DreamVector::Normalize()
{
	float length = Length();
	if (length != 0.0f)
	{
		float inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
	}
	return *this;
}

//点乘
float DreamVector::Dot(DreamVector v) const
{
	return x * v.x + y*v.y + z*v.z;
}

//叉乘
DreamVector DreamVector::Cross(DreamVector rhs) const
{
    //b1 b2 b3
    //c1 c2 c3
    //b2c3 - b3c2, - (b1c3 -  b3c1), b1c2 - b2c1
	float m1 = y * rhs.z - z * rhs.y;
	float m2 = z * rhs.x - x * rhs.z;
	float m3 = x * rhs.y - y * rhs.x;
	return DreamVector(m1, m2, m3, 0.0f);
}

//v1 == v2 ?
bool DreamVector::operator==(const DreamVector& rhs) const
{
	if (abs(x - rhs.x) <= 0.000001f 
		&& abs(y - rhs.y) <= 0.000001f 
		&& abs(z - rhs.z) <= 0.000001f 
		&& abs(w - rhs.w) <= 0.000001f)
		return true;
	else
		return false;
}

//v1 = v2 * m
/*DreamVector DreamVector::operator* (const ZCMatrix& rhs) const
{
	return DreamVector(x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41,
					x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42,
					x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43,
					x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44);
}*/

//分量相乘
DreamVector DreamVector::operator*(const DreamVector& rhs) const
{
	return DreamVector(
		x * rhs.x,
		y * rhs.y,
		z * rhs.z,
		w * rhs.w
	);
}

DreamVector DreamVector::operator*(float factor) const
{
	return DreamVector(
		x * factor,
		y * factor,
		z * factor,
		w
	);
}

//v1 = v2 + v3
DreamVector DreamVector::operator+(const DreamVector& rhs) const
{
	return DreamVector(x + rhs.x, y + rhs.y, z + rhs.z, 0.0f);
}

//v1 = v2 - v3
DreamVector DreamVector::operator-(const DreamVector& rhs) const
{
	return DreamVector(x - rhs.x, y - rhs.y, z - rhs.z, 0.0f);
}

DreamVector DreamVector::operator-() const
{
	return DreamVector(-x, -y - z, w);
}
}
