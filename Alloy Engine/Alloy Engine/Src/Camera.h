#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera(const Vector3 _pos, const Quaternion _rot, const Vector3 _scale,
		float _fov, float _aspect, float _near_clip, float _far_clip);
	Camera(const Vector3 _pos, const Vector3 _look_at , const Vector3 _up,
		float _fov, float _aspect, float _near_clip, float _far_clip);

	~Camera() = default;

	void MoveForward(float _distance);
	void MoveRight(float _distance);
	void MoveUp(float _distance);

	void RotateRight(float _angle);
	void RotateUp(float _angle);

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix() const;

private:
	Matrix m_projection;

	struct
	{
		Matrix view_matrix;
		bool is_outdated;
	} m_view_matrix;
};

