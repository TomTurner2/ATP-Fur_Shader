#pragma once
#include "Matrix.h"


class Vector3
{
public:
	Vector3() = default;
	Vector3(float _x, float _y, float _z);
	~Vector3() = default;

	float x = 0;
	float y = 0;
	float z = 0;

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Up;
	static const Vector3 Right;
	static const Vector3 Forward;

	static const float Epsilon;

	//operator overrides
	Vector3	operator-() const;
	Vector3	operator-(const Vector3& _vector) const;
	Vector3	operator+(const Vector3& _vector) const;
	Vector3	operator*(float _value) const;
	Vector3	operator/(float _value) const;

	Vector3& operator-=(const Vector3& _vector);
	Vector3& operator+=(const Vector3& _vector);
	Vector3& operator*=(float _value);
	Vector3& operator/=(float _value);

	bool operator==(const Vector3& _vector) const;
	bool operator!=(const Vector3& _vector) const;

	//static vector functions
	static Vector3 ComponentProduct(const Vector3& _vector_a, const Vector3& _vector_b);
	static Vector3 Cross(const Vector3& _vector_a, const Vector3& _vector_b);
	static float Dot(const Vector3& _vector_a, const Vector3& _vector_b);
	static float Distance(const Vector3& _vector_a, const Vector3& _vector_b);
	static float DistanceSquared(const Vector3& _vector_a, const Vector3& _vector_b);
	static float Magnitude(Vector3& _vector);
	static float MagnitudeSquared(Vector3& _vector);
	static void Normalise(Vector3& _vector);
	static void SetMagnitude(Vector3& _vector, float _magnitude);
	static void Clamp(Vector3& _vector, float _magnitude);
	static Vector3 Transform(Vector3& _position, Matrix& _matrix);


	//vector functions
	Vector3 Cross(const Vector3& _vector_b) const;
	float Dot(const Vector3& _vector) const;
	float Distance(const Vector3& _vector_b) const;
	float DistanceSquared(const Vector3& _vector_b) const;
	float Magnitude() const;
	float MagnitudeSquared() const;
	Vector3 Normalised() const;
	void SetMagnitude(float _magnitude);
	void Clamp(float _magnitude);

	void ToPolar();
	void ToPolar(float _x, float _y, float _z);
	void ToCartesian(float _radius, float __angle, float _z);
		
	//utility functions
	void Display() const;
};
