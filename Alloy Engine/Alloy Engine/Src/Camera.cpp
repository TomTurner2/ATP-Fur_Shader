#include "Camera.h"


Camera::Camera(const Vector3 _pos, const Quaternion _rot,
	const Vector3 _scale, float _fov, float _aspect, float _near_clip, float _far_clip)
{
	m_transform = Transform(_pos, _rot, _scale);
	m_projection = Matrix::PerspectiveProjection(_fov, _aspect, _near_clip, _far_clip);
	m_view_matrix.first = true;// Set matrix to dirty.
}


Camera::Camera(const Vector3 _pos, const Vector3 _look_at, const Vector3 _up,
	float _fov, float _aspect, float _near_clip, float _far_clip)
{
	m_transform = Transform(_pos, Quaternion::Identity, Vector3::One);
	m_forward = (_look_at - m_transform.GetPosition()).Normalised();// Forward is target direction.	
	m_up = _up;
	m_right = Vector3::Cross(m_up, m_forward).Normalised();// Calculate right.
	m_projection = Matrix::PerspectiveProjection(_fov, _aspect, _near_clip, _far_clip);// Create camera projection.
}


void Camera::Tick(GameData& _game_data)
{
	ProcessForward(_game_data);
	ProcessBackward(_game_data);
	ProcessLeft(_game_data);
	ProcessRight(_game_data);
	ProcessUp(_game_data);
	ProcessDown(_game_data);	
}


void Camera::ProcessForward(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::FORWARD, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::FORWARD, InputManager::PRESSED))
	{
		MoveForward(m_move_speed * _game_data.delta_time);
	}
}


void Camera::ProcessBackward(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::BACKWARD, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::BACKWARD, InputManager::PRESSED))
	{
		MoveForward(-m_move_speed  * _game_data.delta_time);
	}
}


void Camera::ProcessLeft(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::LEFT, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::LEFT, InputManager::PRESSED))
	{
		MoveRight(-m_move_speed  * _game_data.delta_time);
	}
}


void Camera::ProcessRight(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::RIGHT, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::RIGHT, InputManager::PRESSED))
	{
		MoveRight(m_move_speed  * _game_data.delta_time);
	}
}


void Camera::ProcessUp(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::UP, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::UP, InputManager::PRESSED))
	{
		MoveUp(m_move_speed  * _game_data.delta_time);
	}
}


void Camera::ProcessDown(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::DOWN, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::DOWN, InputManager::PRESSED))
	{
		MoveUp(-m_move_speed  * _game_data.delta_time);
	}
}


//TODO make camera movement relative
void Camera::MoveForward(float _distance)
{
	Vector3 new_pos = m_transform.GetPosition();
	new_pos += Vector3::Forward * _distance;
	m_transform.SetPosition(new_pos);

	m_view_matrix.first = true;
}


void Camera::MoveRight(float _distance)
{
	Vector3 new_pos = m_transform.GetPosition();
	new_pos += Vector3::Right * _distance;
	m_transform.SetPosition(new_pos);

	m_view_matrix.first = true;
}


void Camera::MoveUp(float _distance)
{
	Vector3 new_pos = m_transform.GetPosition();
	new_pos += Vector3::Up * _distance;
	m_transform.SetPosition(new_pos);

	m_view_matrix.first = true;
}


Matrix Camera::GetViewMatrix()
{
	if (m_view_matrix.first)// If dirty.
	{
		Vector3 target = m_transform.GetPosition();
		target += m_transform.GetRelativeDir(Vector3::Forward);//Place look target just in front of the camera.

		m_view_matrix.second = Matrix::LookAt(m_transform.GetPosition(), target,
			m_transform.GetRelativeDir(Vector3::Up));

		m_view_matrix.first = false;// Matrix is Clean.
	}

	return m_view_matrix.second;
}


Matrix Camera::GetProjectionMatrix() const
{
	return m_projection;
}
