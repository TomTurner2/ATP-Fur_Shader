#pragma once
#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"


struct Transform
{
public:
	Transform();
	Transform(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale);
	Transform(const Transform& _transform) = default;
	~Transform() = default;

	Vector3 GetPosition() const;
	Vector3 GetScale() const;
	Quaternion GetRotation() const;

	void SetRelativeDir(const Vector3& _dir, Vector3& _target);
	void SetPosition(Vector3& _pos);
	void SetScale(Vector3& _scale);
	void SetRotation(Quaternion& _rot);

	Vector3 GetRelativeDir(const Vector3 _dir) const;
	Matrix GetTransformMatrix() const;
	Transform Inverse() const;

	static Transform FromTransformMatrix(const Matrix& m);
	static Transform Lerp(const Transform& _transform_a, const Transform& _transform_b, float _percent);

	const static Transform Identity;

private:
	Vector3 m_pos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
};