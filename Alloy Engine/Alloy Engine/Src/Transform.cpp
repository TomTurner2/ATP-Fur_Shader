#include "Transform.h"


const Transform Transform::Identity = Transform();


Transform::Transform(): m_pos(0.f, 0.f, 0.f), m_rot(), m_scale(1.f, 1.f, 1.f) {}


Transform::Transform(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale)
	: m_pos(_pos)
	, m_rot(_rot)
	, m_scale(_scale)
{}


Vector3 Transform::GetPosition() const
{
	return m_pos;
}


Vector3 Transform::GetScale() const
{
	return m_scale;
}


Quaternion Transform::GetRotation() const
{
	return m_rot;
}


void Transform::SetRelativeDir(const Vector3& _dir, Vector3& _target)
{
	//Matrix look = Matrix::LookAt(_dir, _target, Vector3::Up);
}


void Transform::SetPosition(Vector3& _pos)
{
	m_pos = _pos;
}


void Transform::SetScale(Vector3& _scale)
{
	m_scale = _scale;
}


void Transform::SetRotation(Quaternion& _rot)
{
	m_rot = _rot;
}


Vector3 Transform::GetRelativeDir(const Vector3 _dir) const
{
	Vector3 dir = _dir;
	dir = Vector3::Transform(dir, Quaternion::ToMatrix(m_rot));
	return dir;
}


Matrix Transform::GetTransformMatrix() const
{
	Matrix m;

	m._11 = m_scale.x * (1.f - 2.f * m_rot.y * m_rot.y - 2.f * m_rot.z * m_rot.z);
	m._12 = m_scale.y * (2.f * m_rot.x * m_rot.y - 2.f * m_rot.z * m_rot.w);
	m._13 = m_scale.z * (2.f * m_rot.x * m_rot.z + 2.f * m_rot.y * m_rot.w);
	m._14 = m_pos.x;

	m._21 = m_scale.x * (2.f * m_rot.x * m_rot.y + 2.f * m_rot.z * m_rot.w);
	m._22 = m_scale.y * (1.f - 2.f * m_rot.x * m_rot.x - 2.f * m_rot.z * m_rot.z);
	m._23 = m_scale.z * (2.f * m_rot.y * m_rot.z - 2.f * m_rot.x * m_rot.w);
	m._24 = m_pos.y;

	m._31 = m_scale.x * (2.f *  m_rot.x *  m_rot.z - 2.f *  m_rot.y *  m_rot.w);
	m._32 = m_scale.y * (2.f *  m_rot.y *  m_rot.z + 2.f *  m_rot.x *  m_rot.w);
	m._33 = m_scale.z * (1.f - 2.f *  m_rot.x *  m_rot.x - 2.f *  m_rot.y *  m_rot.y);
	m._34 = m_pos.z;

	m._41 = 0.f;
	m._42 = 0.f;
	m._43 = 0.f;
	m._44 = 1.f;

	return m;
}


Transform Transform::Inverse() const
{
	return Transform({ -m_pos, m_rot.Inverse(), Vector3(1.f / m_scale.x, 1.f / m_scale.y, 1.f / m_scale.z) });
}


Transform Transform::Lerp(const Transform& _transform_a, const Transform& _transform_b, float _ratio)
{
	return Transform(
		_transform_a.m_pos * (1.f - _ratio) + _transform_b.m_pos * _ratio,
		Quaternion(
			_transform_a.m_rot.w * (1.f - _ratio) + _transform_b.m_rot.w * _ratio,
			_transform_a.m_rot.x * (1.f - _ratio) + _transform_b.m_rot.x * _ratio,
			_transform_a.m_rot.y * (1.f - _ratio) + _transform_b.m_rot.y * _ratio,
			_transform_a.m_rot.z * (1.f - _ratio) + _transform_b.m_rot.z * _ratio
		),
		_transform_a.m_scale * (1.f - _ratio) + _transform_b.m_scale * _ratio
	);
}


Transform Transform::FromTransformMatrix(const Matrix& _matrix)
{
	Vector3 pos(_matrix._14, _matrix._24, _matrix._34);

	float scale_x = Vector3(_matrix._11, _matrix._21, _matrix._31).Magnitude();
	float scale_y = Vector3(_matrix._12, _matrix._22, _matrix._32).Magnitude();
	float scale_z = Vector3(_matrix._13, _matrix._23, _matrix._33).Magnitude();

	Quaternion rotation = Quaternion::FromMatrix(_matrix);

	Vector3 scale = Vector3(scale_x, scale_y, scale_z);

	return Transform(pos, rotation, scale);
}