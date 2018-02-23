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

	void Tick(GameData& _game_data) override;
	

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix() const;

private:
	Matrix m_projection;
	std::pair<bool, Matrix> m_view_matrix;//dirty pattern to reduce unneccessary calculations
	float m_move_speed = 100;

	Vector3 m_forward = Vector3::Forward;
	Vector3 m_up = Vector3::Up;
	Vector3 m_right = Vector3::Right;

	void ProcessForward(GameData& _game_data);
	void ProcessBackward(GameData& _game_data);
	void ProcessLeft(GameData& _game_data);
	void ProcessRight(GameData& _game_data);
	void ProcessUp(GameData& _game_data);
	void ProcessDown(GameData& _game_data);

	void MoveForward(float _distance);
	void MoveRight(float _distance);
	void MoveUp(float _distance);
};

