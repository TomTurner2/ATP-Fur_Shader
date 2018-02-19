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

void Game::SwitchMaterials() const
{
	if (m_default_material_on)
	{
		m_model->SetAllModelMaterials(m_fur_gs_material);
		return;
	}

	m_model->SetAllModelMaterials(m_standard_material);
}


#pragma region Creation Functions
void Game::CreateLight()
{
	m_light.position = Vector3(0, 6, -115.0f);
	m_light.colour[0] = 1;
	m_light.colour[1] = 1;
	m_light.colour[2] = 1;
	m_light.intensity = 1;
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

	material_params.roughness = 1;
	material_params.specular = 1;
	material_params.diffuse[0] = 1; 
	material_params.diffuse[1] = 1;
	material_params.diffuse[2] = 1;

	m_standard_material = new Material();
	m_standard_material->CreateShaders("Standard_Material_Vertex_Shader.cso",
		"Standard_Material_Pixel_Shader.cso", _renderer);

	m_fur_parameters.fur_mask_multiplier = 1;
	m_fur_parameters.base_clip = 0.9f;
	m_fur_parameters.end_clip = 0.9f;
	m_fur_parameters.max_fur_length = 4;
	m_fur_parameters.layer_count = 5;
	m_fur_parameters.gravity = Vector3::Zero;
	

	m_fur_gs_material = new FurMaterial();
	m_fur_gs_material->CreateShaders("Fur_Vertex_Shader.cso",
		"Fur_Pixel_Shader.cso", _renderer, "Fur_Shell_Geometry_Shader.cso");
	m_fur_gs_material->LoadStandardTextures("./Big_Cat_Albedo.png",
		"./Big_Cat_Roughness.png", "./Big_Cat_Specular.png", _renderer);
	m_fur_gs_material->LoadFurTextures(_renderer, "./Big_Cat_Fur_Mask.png", "./Big_Cat_Fur_Alpha.png");
	m_fur_gs_material->SetFurParameters(m_fur_parameters);
	m_model->SetAllModelMaterials(m_standard_material);
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
	m_game_data->input->BindKey(QUIT, "X");
	m_game_data->input->BindKey(SWITCH, "F");
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
	TwAddVarRW(m_bar, "light_intensity", TW_TYPE_FLOAT, &m_light.intensity, "");
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

	TwAddVarRW(m_bar, "Max_Length", TW_TYPE_FLOAT, &m_fur_parameters.max_fur_length, "");
	TwAddVarRW(m_bar, "Layer_Count", TW_TYPE_INT8, &m_fur_parameters.layer_count, "");
	TwAddVarRW(m_bar, "Base_Clip", TW_TYPE_FLOAT, &m_fur_parameters.base_clip, "");
	TwAddVarRW(m_bar, "End_Clip", TW_TYPE_FLOAT, &m_fur_parameters.end_clip, "");

	TwDefine("Fur_Shader_Prototype/Mask_Multiplier   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Max_Length   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Layer_Count   step=1 ");
	TwDefine("Fur_Shader_Prototype/Base_Clip   step=0.01 ");
	TwDefine("Fur_Shader_Prototype/End_Clip   step=0.01 ");

	TwAddVarRW(m_bar, "Gravity_X", TW_TYPE_FLOAT, &m_fur_parameters.gravity.x, "");
	TwAddVarRW(m_bar, "Gravity_Y", TW_TYPE_FLOAT, &m_fur_parameters.gravity.y, "");
	TwAddVarRW(m_bar, "Gravity_Z", TW_TYPE_FLOAT, &m_fur_parameters.gravity.z, "");

	TwDefine("Fur_Shader_Prototype/Gravity_X   group=Gravity_Direction");
	TwDefine("Fur_Shader_Prototype/Gravity_Y   group=Gravity_Direction");
	TwDefine("Fur_Shader_Prototype/Gravity_Z   group=Gravity_Direction");

	TwAddVarRW(m_bar, "Fur on", TW_TYPE_BOOLCPP, &m_default_material_on, "");
}
#pragma endregion


void Game::Tick()
{
	if (m_game_data->input->GetGameAction(GameAction::QUIT, InputManager::PRESSED))
		exit(0);

	SwitchMaterials();

	m_game_data->delta_time = CalculateDeltaTime();
	m_camera->Tick(*m_game_data.get());
	m_model->Tick(*m_game_data.get());
	m_standard_material->SetMaterialParams(material_params);
	m_fur_gs_material->SetMaterialParams(material_params);
	m_fur_gs_material->SetFurParameters(m_fur_parameters);
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
