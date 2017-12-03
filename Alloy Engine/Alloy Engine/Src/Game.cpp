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
	m_light.position = Vector3(0, 6, -115.0f);
	m_light.colour[0] = 1;
	m_light.colour[1] = 1;
	m_light.colour[2] = 1;
}


void Game::CreateCamera(Renderer & _renderer)
{
	m_camera = std::make_unique<Camera>(Vector3(-3, -27, -138), Quaternion(Vector3::Forward, TMath::Radians(0)),
		Vector3::One, TMath::Radians(70), _renderer.GetViewportAspectRatio(), 5, 500);
}


void Game::CreateModel(Renderer& _renderer)
{
	m_model = std::make_unique<Model>();
	m_model->LoadModel("./Big_Cat.obj", _renderer);

	material_params.roughness = 0.6f;
	material_params.specular = 0.2f;

	m_fur_material = new FurMaterial();
	m_fur_material->CreateShaders("Fur_Vertex_Shader.cso",
		"Fur_Pixel_Shader.cso", _renderer, "Fur_Shell_Geometry_Shader.cso");

	m_model->SetAllModelMaterials(m_fur_material);
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
	TwAddVarRW(m_bar, "Light Colour", TW_TYPE_COLOR3F, &m_light.colour, " colormode=hls ");
	TwAddVarRW(m_bar, "light_X", TW_TYPE_FLOAT, &m_light.position.x, "");
	TwAddVarRW(m_bar, "light_Y", TW_TYPE_FLOAT, &m_light.position.y, "");
	TwAddVarRW(m_bar, "light_Z", TW_TYPE_FLOAT, &m_light.position.z, "");

	//move light position to group
	TwDefine("Fur_Shader_Prototype/light_X   group=Light_Position");
	TwDefine("Fur_Shader_Prototype/light_Y   group=Light_Position");
	TwDefine("Fur_Shader_Prototype/light_Z   group=Light_Position");


	TwAddVarRW(m_bar, "Diffuse", TW_TYPE_COLOR3F, &material_params.diffuse, " colormode=hls ");
	TwAddVarRW(m_bar, "Rough", TW_TYPE_FLOAT, &material_params.roughness, "");
	TwAddVarRW(m_bar, "Specular", TW_TYPE_FLOAT, &material_params.specular, "");

	TwDefine("Fur_Shader_Prototype/Rough  min=0.1 max=1 ");
	TwDefine("Fur_Shader_Prototype/Specular  min=0.1 max=1 ");

	TwDefine("Fur_Shader_Prototype/Rough   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Specular   step=0.1 ");
}
#pragma endregion


void Game::Tick()
{
	m_game_data->delta_time = CalculateDeltaTime();
	m_camera->Tick(*m_game_data.get());
	m_model->Tick(*m_game_data.get());
	m_fur_material->SetMaterialParams(material_params);
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
