#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera(const Vector3 _pos, const Quaternion _rot, const Vector3 _scale);
	Camera(const Vector3 _pos, const Vector3 _look_at , const Vector3 _up);

	~Camera() = default;

	void MoveForward(float _distance);
	void MoveRight(float _distance);
	void MoveUp(float _distance);

	void RotateRight(float _angle);
	void RotateUp(float _angle);

	Matrix GetViewMatrix();

private:

	struct
	{
		Matrix view_matrix;
		bool is_outdated;
	} m_view_matrix;
};

