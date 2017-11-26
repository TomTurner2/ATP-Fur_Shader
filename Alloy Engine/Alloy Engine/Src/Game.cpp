#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
#include "TMath.h"
#include "Model.h"
#include <AntTweakBar.h>


Game::Game(Renderer& _renderer, InputManager& _input)
{
	CreateLight();
	CreateCamera(_renderer);
	CreateModel(_renderer);
	CreateGameData(_input);
	CreateDebugUI();
	BindParamsToUI();
}


#pragma region Creation Functions
void Game::CreateLight()
{
	m_light.position = Vector3(50.0f, 50.0f, -250.0f);
	m_light.colour[0] = 1;
	m_light.colour[1] = 1;
	m_light.colour[2] = 1;
}


void Game::CreateCamera(Renderer & _renderer)
{
	m_camera = std::make_unique<Camera>(Vector3(0, -75, -190), Quaternion(Vector3::Forward, TMath::Radians(0)),
		Vector3::One, TMath::Radians(70), _renderer.GetViewportAspectRatio(), 5, 500);
}


void Game::CreateModel(Renderer& _renderer)
{
	m_model = std::make_unique<Model>();
	m_model->LoadModel("./Axe.obj", _renderer);
	m_model->LoadAllModelMaterials("Standard_Material_Vertex_Shader.cso",
		"Standard_Material_Pixel_Shader.cso", _renderer);//load shader
	m_model->GetTransform().SetScale(Vector3(1, 1, 1));
	m_model->GetTransform().SetRotation(Quaternion(Vector3::Up, 90));
}


void Game::CreateGameData(InputManager& _input)
{
	m_game_data = std::make_unique<GameData>();

	m_game_data->input = &_input;
	m_game_data->input->BindKey(FORWARD, "W");
	m_game_data->input->BindKey(BACKWARD, "S");
	m_game_data->input->BindKey(LEFT, "A");
	m_game_data->input->BindKey(RIGHT, "D");
	m_game_data->input->BindKey(UP, "E");
	m_game_data->input->BindKey(DOWN, "Q");
}


void Game::CreateDebugUI()
{
	m_bar = TwNewBar("Fur_Shader_Prototype");
	TwDefine("Fur_Shader_Prototype label='Fur Shader Prototype' ");
	TwDefine("Fur_Shader_Prototype color='46 53 49' text=light ");
	TwDefine("Fur_Shader_Prototype alpha=255");
}


void Game::BindParamsToUI()
{
	//bing light to UI
	TwAddVarRW(m_bar, "Light Colour", TW_TYPE_COLOR3F, &m_light.colour, " colormode=rgb ");
	TwAddVarRW(m_bar, "light_X", TW_TYPE_FLOAT, &m_light.position.x, "");
	TwAddVarRW(m_bar, "light_Y", TW_TYPE_FLOAT, &m_light.position.y, "");
	TwAddVarRW(m_bar, "light_Z", TW_TYPE_FLOAT, &m_light.position.z, "");

	//move light position to group
	TwDefine("Fur_Shader_Prototype/light_X   group=Light_Position");
	TwDefine("Fur_Shader_Prototype/light_Y   group=Light_Position");
	TwDefine("Fur_Shader_Prototype/light_Z   group=Light_Position");
}
#pragma endregion


void Game::Tick()
{
	m_game_data->delta_time = CalculateDeltaTime();
	m_camera->Tick(*m_game_data.get());
	m_model->Tick(*m_game_data.get());
	m_model->SetAllMaterialParams(material);
}


void Game::Draw(Renderer& _renderer) const
{
	UpdateRenderData(_renderer, *m_camera);
	m_model->Draw(_renderer);
}


void Game::UpdateRenderData(Renderer& _renderer, Camera& _camera) const
{
	_renderer.GetRenderData()->camera_view_matrix = _camera.GetViewMatrix();
	_renderer.GetRenderData()->camera_projection_matrix = _camera.GetProjectionMatrix();
	_renderer.GetRenderData()->camera_pos = _camera.GetTransform().GetPosition();
	_renderer.GetRenderData()->light = m_light;
}


float Game::CalculateDeltaTime()
{
	DWORD currentTime = GetTickCount();
	float  dt = min(static_cast<float>((currentTime - m_playTime) / 1000.0f), 0.1f);
	m_playTime = currentTime;
	return dt;
}
