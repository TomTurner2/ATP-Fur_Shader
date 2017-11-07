#include "Vector3.h"
#include <limits>
#include <iostream>


//helper const values
const Vector3 Vector3::Zero = Vector3(0, 0, 0);
const Vector3 Vector3::One = Vector3(1, 1, 1);
const Vector3 Vector3::Up = Vector3(0, 1, 0);
const Vector3 Vector3::Right = Vector3(1, 0, 0);
const Vector3 Vector3::Forward = Vector3(0, 0, 1);
const float Vector3::Epsilon = std::numeric_limits<float>::epsilon();


Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}//three value constructor


Vector3 Vector3::ComponentProduct(const Vector3& _vector_a, const Vector3& _vector_b)
{
	return Vector3(_vector_a.x * _vector_b.x, _vector_a.y * _vector_b.y,
		_vector_a.z * _vector_b.z);
}


Vector3 Vector3::Cross(const Vector3& _vector_a, const Vector3& _vector_b)
{
	return Vector3(_vector_a.y * _vector_b.z - _vector_a.z * _vector_b.y,
		_vector_a.z * _vector_b.x - _vector_a.x * _vector_b.z, _vector_a.x * _vector_b.y - _vector_a.y * _vector_b.x);
}


float Vector3::Dot(const Vector3& _vector_a, const Vector3& _vector_b)
{
	return _vector_a.x * _vector_b.x + _vector_a.y *  _vector_b.y + _vector_a.z *  _vector_b.z;
}


Vector3 Vector3::Cross(const Vector3& _vector) const
{
	return Vector3(y * _vector.z - z * _vector.y,
		z * _vector.x - x * _vector.z, x * _vector.y - y * _vector.x);
}


float Vector3::Dot(const Vector3& _vector) const
{
	return x * _vector.x + y * _vector.y + z * _vector.z;
}


float Vector3::Distance(const Vector3& _vector_a, const Vector3& _vector_b)
{
	float dist_x = _vector_b.x - _vector_a.x;
	float dist_y = _vector_b.y - _vector_a.y;
	float dist_z = _vector_b.z - _vector_a.z;

	return sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
}


float Vector3::DistanceSquared(const Vector3& _vector_a, const Vector3& _vector_b)
{
	float dist_x = _vector_b.x - _vector_a.x;
	float dist_y = _vector_b.y - _vector_a.y;
	float dist_z = _vector_b.z - _vector_a.z;

	return dist_x * dist_x + dist_y * dist_y + dist_z * dist_z;
}


float Vector3::Distance(const Vector3& _vector) const
{
	float dist_x = _vector.x - x;
	float dist_y = _vector.y - y;
	float dist_z = _vector.z - z;

	return dist_x * dist_x + dist_y * dist_y + dist_z * dist_z;
}


float Vector3::DistanceSquared(const Vector3& _vector) const
{
	float dist_x = _vector.x - x;
	float dist_y = _vector.y - y;
	float dist_z = _vector.z - z;

	return dist_x * dist_x + dist_y * dist_y + dist_z * dist_z;
}


float Vector3::Magnitude(Vector3& _vector)
{
	return sqrt(_vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z);
}


float Vector3::MagnitudeSquared(Vector3& _vector)
{
	return _vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z;
}


float Vector3::Magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}


float Vector3::MagnitudeSquared() const
{
	return x * x + y * y + z * z;
}


void Vector3::Normalise(Vector3& _vector)
{
	_vector /= _vector.Magnitude();
}


Vector3 Vector3::Normalised() const
{
	return Vector3(x, y, z) /= Magnitude();
}
	

void Vector3::SetMagnitude(Vector3& _vector, float _magnitude)
{
	_vector *= _magnitude / Magnitude(_vector);
}


void Vector3::Clamp(Vector3& _vector, float _magnitude)
{
	float mag = MagnitudeSquared(_vector);

	if (mag <= _magnitude * _magnitude)
		return;

	_vector *= _magnitude / sqrt(mag);
}


Vector3 Vector3::Transform(Vector3& _position, Matrix& _matrix)
{
	Vector3 transformed_vector = Vector3::Zero;
	transformed_vector.x = (_position.x * _matrix._11) + (_position.y * _matrix._21) +
		(_position.z * _matrix._31) + _matrix._41;

	transformed_vector.y = (_position.x * _matrix._12) + (_position.y * _matrix._22) +
		(_position.z * _matrix._32) + _matrix._42;

	transformed_vector.z = (_position.x * _matrix._13) + (_position.y * _matrix._23) +
		(_position.z * _matrix._33) + _matrix._43;

	return transformed_vector;
}


void Vector3::SetMagnitude(float _magnitude)
{
	Vector3 mag = Vector3(x, y, z);
	mag *= _magnitude / Magnitude();
	x = mag.x;
	y = mag.y;
	z = mag.z;
}


void Vector3::Clamp(float _magnitude)
{
	Vector3 clamped = Vector3(x, y, z);
	float mag = MagnitudeSquared(clamped);

	if (mag <= _magnitude * _magnitude)
		return;

	clamped *= _magnitude / sqrt(mag);
	x = clamped.x;
	y = clamped.y;
	z = clamped.z;
}


void Vector3::ToPolar()
{
	float temp_x = x;
	float temp_y = y;

	x = atan2(temp_y, temp_x);
	y = sqrt(temp_x * temp_x + temp_y * temp_y);
}


void Vector3::ToPolar(float _x, float _y, float _z)
{
	x = atan2(_y, _x);
	y = sqrt(_x * _x + _x * _x);
	z = _z;
}


void Vector3::ToCartesian(float _radius, float _angle, float _z)
{
	x = _radius * cos(_angle);
	y = _radius * sin(_angle);
	z = _z;
}


#pragma region Operator Overloads
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}


Vector3 Vector3::operator-(const Vector3& _vector) const
{
	return Vector3(x - _vector.x, y - _vector.y, x - _vector.z);
}


Vector3 Vector3::operator+(const Vector3& _vector) const
{
	return Vector3(x + _vector.x, y + _vector.y, x + _vector.z);
}


Vector3 Vector3::operator*(float _value) const
{
	return Vector3(x * _value, y * _value, z * _value);
}


Vector3 Vector3::operator/(float _value) const
{
	return Vector3(x / _value, y / _value, z / _value);
}


Vector3& Vector3::operator-=(const Vector3& _vector)
{
	x -= _vector.x;
	y -= _vector.y;
	z -= _vector.z;
	return *this;
}


Vector3& Vector3::operator+=(const Vector3& _vector)
{
	x += _vector.x;
	y += _vector.y;
	z += _vector.z;
	return *this;
}


Vector3& Vector3::operator*=(float _value)
{
	x *= _value;
	y *= _value;
	z *= _value;
	return *this;
}


Vector3& Vector3::operator/=(float _value)
{
	x /= _value;
	y /= _value;
	z /= _value;
	return *this;
}


bool Vector3::operator==(const Vector3& _vector) const
{
	return (x > _vector.x - Vector3::Epsilon && x < _vector.x + Vector3::Epsilon &&
		y > _vector.y - Vector3::Epsilon && y < _vector.y + Vector3::Epsilon &&
		z > _vector.z - Vector3::Epsilon && z < _vector.z + Vector3::Epsilon);//epsilon to avoid floating point inaccuracy
}


bool Vector3::operator!=(const Vector3& _vector) const
{
	return (x < _vector.x - Vector3::Epsilon || x > _vector.x + Vector3::Epsilon ||
		y < _vector.y - Vector3::Epsilon || y > _vector.y + Vector3::Epsilon ||
		z < _vector.z - Vector3::Epsilon || z > _vector.z + Vector3::Epsilon);
}
#pragma endregion 


void Vector3::Display() const
{
	std::cout << "X: " << x << "\t Y: " << y << "\t Z: " << z << std::endl;
}
