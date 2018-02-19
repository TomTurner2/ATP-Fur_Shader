#include "Camera.h"


Camera::Camera(const Vector3 _pos, const Quaternion _rot,
	const Vector3 _scale, float _fov, float _aspect, float _near_clip, float _far_clip)
{
	m_transform = Transform(_pos, _rot, _scale);
	m_projection = Matrix::PerspectiveProjection(_fov, _aspect, _near_clip, _far_clip);
	m_view_matrix.first = true;
}


Camera::Camera(const Vector3 _pos, const Vector3 _look_at, const Vector3 _up,
	float _fov, float _aspect, float _near_clip, float _far_clip)
{
	m_transform = Transform(_pos, Quaternion::Identity, Vector3::One);
	m_forward = (_look_at - m_transform.GetPosition()).Normalised();	
	m_up = _up;
	m_right = Vector3::Cross(m_up, m_forward).Normalised();
	m_projection = Matrix::PerspectiveProjection(_fov, _aspect, _near_clip, _far_clip);
}


void Camera::Tick(GameData& _game_data)
{
	if (_game_data.input->GetGameAction(GameAction::FORWARD, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::FORWARD, InputManager::PRESSED))
	{
		system("cls");
		printf("Input Dir: fwd \n");
		MoveForward(m_move_speed * _game_data.delta_time);

		m_transform.GetPosition().Display();
		printf("Veiw matrix \n");
		m_view_matrix.second.Display();
		printf("\nProjection matrix\n");
		m_projection.Display();
	}

	if (_game_data.input->GetGameAction(GameAction::BACKWARD, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::BACKWARD, InputManager::PRESSED))
	{
		system("cls");
		printf("Input Dir: back \n");
		MoveForward(-m_move_speed  * _game_data.delta_time);

		m_transform.GetPosition().Display();
		printf("View matrix \n");
		m_view_matrix.second.Display();
		printf("\nProjection matrix\n");
		m_projection.Display();
	}

	if (_game_data.input->GetGameAction(GameAction::LEFT, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::LEFT, InputManager::PRESSED))
	{
		system("cls");
		printf("Input Dir: left \n");
		MoveRight(-m_move_speed  * _game_data.delta_time);

		m_transform.GetPosition().Display();
		printf("View matrix \n");
		m_view_matrix.second.Display();
		printf("\nProjection matrix\n");
		m_projection.Display();
	}

	if (_game_data.input->GetGameAction(GameAction::RIGHT, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::RIGHT, InputManager::PRESSED))
	{
		system("cls");
		printf("Input Dir: right \n");
		MoveRight(m_move_speed  * _game_data.delta_time);

		m_transform.GetPosition().Display();
		printf("View matrix \n");
		m_view_matrix.second.Display();
		printf("\nProjection matrix\n");
		m_projection.Display();
	}


	if (_game_data.input->GetGameAction(GameAction::UP, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::UP, InputManager::PRESSED))
	{
		system("cls");
		printf("Input Dir: up \n");
		MoveUp(m_move_speed  * _game_data.delta_time);

		m_transform.GetPosition().Display();
		printf("View matrix \n");
		m_view_matrix.second.Display();
		printf("\nProjection matrix\n");
		m_projection.Display();
	}


	if (_game_data.input->GetGameAction(GameAction::DOWN, InputManager::HELD) ||
		_game_data.input->GetGameAction(GameAction::DOWN, InputManager::PRESSED))
	{
		system("cls");
		printf("Input Dir: down \n");
		MoveUp(-m_move_speed  * _game_data.delta_time);
		m_transform.GetPosition().Display();
		printf("View matrix \n");
		m_view_matrix.second.Display();
		printf("\nProjection matrix\n");
		m_projection.Display();
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
