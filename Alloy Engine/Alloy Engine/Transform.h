#pragma once
#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"


struct Transform
{
public:
	Vector3 m_pos;
	Quaternion m_rot;
	Vector3 m_scale;

	Transform();
	Transform(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale);
	Transform(const Transform& _transform) = default;
	~Transform() = default;

	Transform operator*(const Transform& _transform) const;	// Combine transforms

	Matrix GetTransformMatrix() const;
	Transform Inverse() const;

	static Transform FromTransformMatrix(const Matrix& m);
	static Transform Lerp(const Transform& _transform_a, const Transform& _transform_b, float _percent);

	const static Transform Identity;
};