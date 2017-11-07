#include "Camera.h"


Camera::Camera(const Vector3 _pos, const Quaternion _rot,
	const Vector3 _scale, float _fov, float _aspect, float _near_clip, float _far_clip)
{
	m_transform = Transform(_pos, _rot, _scale);
	m_projection = Matrix::PerspectiveProjection(_fov, _aspect, _near_clip, _far_clip);
}


Camera::Camera(const Vector3 _pos, const Vector3 _look_at, const Vector3 _up,
	float _fov, float _aspect, float _near_clip, float _far_clip)
{
	//TODO: make transfrom and look at target
	m_projection = Matrix::PerspectiveProjection(_fov, _aspect, _near_clip, _far_clip);
}


void Camera::MoveForward(float _distance)
{
	Vector3 new_pos = m_transform.GetPosition();
	new_pos += m_transform.GetRelativeDir(Vector3::Forward) * _distance;

	m_view_matrix.first = true;
}


void Camera::MoveRight(float _distance)
{
	Vector3 new_pos = m_transform.GetPosition();
	new_pos += m_transform.GetRelativeDir(Vector3::Right) * _distance;

	m_view_matrix.first = true;
}


void Camera::MoveUp(float _distance)
{
	Vector3 new_pos = m_transform.GetPosition();
	new_pos += m_transform.GetRelativeDir(Vector3::Up) * _distance;

	m_view_matrix.first = true;
}


void Camera::RotateRight(float _angle)
{
	//TODO: need to be able to set transform forward and right before rot can be done
}


void Camera::RotateUp(float _angle)
{
	//TODO: need to be able to set transform forward and right before rot can be done
}


Matrix Camera::GetViewMatrix()
{
	if (m_view_matrix.first)
	{
		Vector3 target = m_transform.GetPosition();
		target += m_transform.GetRelativeDir(Vector3::Forward);//place look target just in front of the camera

		m_view_matrix.second = Matrix::LookAt(m_transform.GetPosition(), target,
			m_transform.GetRelativeDir(Vector3::Up));

		m_view_matrix.first = false;
	}

	return m_view_matrix.second;
}


Matrix Camera::GetProjectionMatrix() const
{
	return m_projection;
}
