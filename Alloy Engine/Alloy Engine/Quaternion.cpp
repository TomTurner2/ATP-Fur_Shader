#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix.h"
#include <algorithm>


const Quaternion Quaternion::Identity = Quaternion(0.f, 0.f, 0.f, 1.f);


Quaternion::Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {}


Quaternion::Quaternion(Vector3 _axis, float _angle)	: w(cosf(_angle / 2.f))
	, x(sinf(_angle / 2.f) * _axis.x)
	, y(sinf(_angle / 2.f) * _axis.y)
	, z(sinf(_angle / 2.f) * _axis.z)
{
	Normalize();
}


Quaternion::Quaternion(float W, float X, float Y, float Z)
	: w(W)
	, x(X)
	, y(Y)
	, z(Z)
{
	Normalize();
}


Quaternion Quaternion::Inverse() const
{
	return Quaternion(-w, x, y, z);
}


Quaternion Quaternion::operator*(const Quaternion& _quaternion) const
{
	return Quaternion(
		_quaternion.w * w - _quaternion.x * x - _quaternion.y * y - _quaternion.z * z,
		_quaternion.w * x + _quaternion.x * w - _quaternion.y * z + _quaternion.z * y,
		_quaternion.w * y + _quaternion.x * z + _quaternion.y * w - _quaternion.z * x,
		_quaternion.w * z - _quaternion.x * y + _quaternion.y * x + _quaternion.z * w);
}


Quaternion& Quaternion::operator*=(const Quaternion& _quaternion)
{
	return *this = *this * _quaternion;
}


Quaternion Quaternion::FromMatrix(const Matrix& _matrix)
{
	float s_x = Vector3(_matrix._11, _matrix._21, _matrix._31).Magnitude();
	float s_y = Vector3(_matrix._12, _matrix._22, _matrix._32).Magnitude();
	float s_z = Vector3(_matrix._13, _matrix._23, _matrix._33).Magnitude();

	float w = sqrtf(std::max(0.f, 1.f + _matrix._11 / s_x + _matrix._22 / s_y + _matrix._33 / s_z)) / 2.f;

	float x = std::copysignf(sqrtf(std::max(0.f, 1.f + _matrix._11 / s_x - _matrix._22 /
		s_y - _matrix._33 / s_z)) / 2.f, _matrix._32 - _matrix._23);

	float y = std::copysignf(sqrtf(std::max(0.f, 1.f - _matrix._11 / s_x + _matrix._22 /
		s_y - _matrix._33 / s_z)) / 2.f, _matrix._13 - _matrix._31);

	float z = std::copysignf(sqrtf(std::max(0.f, 1.f - _matrix._11 / s_x - _matrix._22 /
		s_y + _matrix._33 / s_z)) / 2.f, _matrix._21 - _matrix._12);

	return Quaternion(w, x, y, z);
}


void Quaternion::Normalize()
{
	float mag = sqrt(x * x + w * w + y * y + z * z);
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
}