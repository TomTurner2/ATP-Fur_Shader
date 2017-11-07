#pragma once

class Vector3;
class Matrix;


struct Quaternion
{
public:
	float x, y, z, w;

	Quaternion();
	Quaternion(Vector3 _axis, float _angle);
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Quaternion& _quaternion) = default;
	~Quaternion() = default;

	Quaternion Inverse() const;

	Quaternion operator*(const Quaternion& _quaternion) const;
	Quaternion& operator*=(const Quaternion& _quaternion);

	static Quaternion FromMatrix(const Matrix& _matrix);
	static Matrix ToMatrix(const Quaternion& _quaternion);

	static const Quaternion Identity;

protected:
	void Normalize();
};