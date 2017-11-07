#include "Game.h"
#include "Triangle.h"
#include "Camera.h"
#include "Renderer.h"
#include "TMath.h"


Game::Game(Renderer& _renderer, InputManager& _input)
{
	m_camera = std::make_unique<Camera>(Vector3::Zero, Quaternion::Identity,
		Vector3::One, TMath::Radians(80), 1024 / 576, 0.1f, 100);//TODO pass in window dimensions


	m_test_triangle = std::make_unique<Triangle>(_renderer);
	m_game_data = std::make_unique<GameData>();

	m_game_data->input = &_input;
	m_game_data->input->BindKey(GameAction::FORWARD, "W");
	m_game_data->input->BindKey(GameAction::BACKWARD, "S");
	m_game_data->input->BindKey(GameAction::LEFT, "A");
	m_game_data->input->BindKey(GameAction::RIGHT, "D");
}


void Game::Tick()
{
	if (m_game_data->input->GetGameAction(GameAction::FORWARD, InputManager::HELD) ||
		m_game_data->input->GetGameAction(GameAction::FORWARD, InputManager::PRESSED))
		printf("fwd ");

	if (m_game_data->input->GetGameAction(GameAction::BACKWARD, InputManager::HELD) ||
			m_game_data->input->GetGameAction(GameAction::BACKWARD, InputManager::PRESSED))
			printf("back ");

	if (m_game_data->input->GetGameAction(GameAction::LEFT, InputManager::HELD) ||
		m_game_data->input->GetGameAction(GameAction::LEFT, InputManager::PRESSED))
		printf("left ");

	if (m_game_data->input->GetGameAction(GameAction::RIGHT, InputManager::HELD) ||
		m_game_data->input->GetGameAction(GameAction::RIGHT, InputManager::PRESSED))
		printf("right ");

	//TODO: calculate dt and assign to fixed dt
}


void Game::Draw(Renderer& _renderer) const
{
	//update the camera info for rendering
	UpdateCurrentRenderCamera(_renderer, *m_camera);

	m_test_triangle->Draw(_renderer);
}


void Game::UpdateCurrentRenderCamera(Renderer& _renderer, Camera& _camera) const
{
	_renderer.GetRenderData()->camera_view_matrix = _camera.GetViewMatrix();
	_renderer.GetRenderData()->camera_projection_matrix = _camera.GetProjectionMatrix();
	_renderer.GetRenderData()->camera_pos = _camera.GetTransform().GetPosition();
}
