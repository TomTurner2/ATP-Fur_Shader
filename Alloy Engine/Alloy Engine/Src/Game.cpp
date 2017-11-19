#include "Game.h"
#include "Triangle.h"
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
#include "TMath.h"
#include "Model.h"
#include <AntTweakBar.h>


Game::Game(Renderer& _renderer, InputManager& _input)
{
	m_camera = std::make_unique<Camera>(Vector3(0, -75, -190), Quaternion(Vector3::Forward, TMath::Radians(0)),
		Vector3::One, TMath::Radians(70), _renderer.GetViewportAspectRatio(), 5, 500);

	m_bar = TwNewBar("Hair_and_Fur");
	TwDefine("Hair_and_Fur color='46 53 49' text=light ");
	TwDefine("Hair_and_Fur alpha=255");

	//model loading test
	m_model = std::make_unique<Model>();
	m_model->LoadModel("./Axe.obj", _renderer);
	m_model->LoadAllModelMaterials("Standard_Material_Vertex_Shader.cso",
		"Standard_Material_Pixel_Shader.cso", _renderer);//load shader
	m_model->GetTransform().SetScale(Vector3(1, 1, 1));
	m_model->GetTransform().SetRotation(Quaternion(Vector3::Up, 90));
	m_game_data = std::make_unique<GameData>();

	m_game_data->input = &_input;
	m_game_data->input->BindKey(FORWARD, "W");
	m_game_data->input->BindKey(BACKWARD, "S");
	m_game_data->input->BindKey(LEFT, "A");
	m_game_data->input->BindKey(RIGHT, "D");
	m_game_data->input->BindKey(UP, "E");
	m_game_data->input->BindKey(DOWN, "Q");
}


void Game::Tick()
{
	m_game_data->delta_time = CalculateDeltaTime();

	m_camera->Tick(*m_game_data.get());
	m_model->Tick(*m_game_data.get());
}


void Game::Draw(Renderer& _renderer) const
{
	//update the camera info for rendering
	UpdateCurrentRenderCamera(_renderer, *m_camera);
	m_model->Draw(_renderer);//test
}


void Game::UpdateCurrentRenderCamera(Renderer& _renderer, Camera& _camera) const
{
	_renderer.GetRenderData()->camera_view_matrix = _camera.GetViewMatrix();
	_renderer.GetRenderData()->camera_projection_matrix = _camera.GetProjectionMatrix();
	_renderer.GetRenderData()->camera_pos = _camera.GetTransform().GetPosition();
}


float Game::CalculateDeltaTime()
{
	DWORD currentTime = GetTickCount();
	float  dt = min(static_cast<float>((currentTime - m_playTime) / 1000.0f), 0.1f);
	m_playTime = currentTime;
	return dt;
}
