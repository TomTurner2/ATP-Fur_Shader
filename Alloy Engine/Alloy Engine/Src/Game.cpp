#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
#include "TMath.h"
#include "Model.h"
#include "TextureSet.h"


Game::Game(Renderer& _renderer, InputManager& _input)
{
	CreateLight();
	CreateCamera(_renderer);
	CreateMaterials(_renderer);
	CreateModels(_renderer);
	CreateGameData(_input);
	SetKeyBindings();
	m_demo_ui = std::make_unique<FurDemoUI>(*this);
}


void Game::SwitchMaterials()
{
	m_default_material_on = !m_default_material_on;
	if (m_default_material_on)
	{
		m_big_cat->SetAllModelMaterials(m_fur_gs_material);
		m_sphere->SetAllModelMaterials(m_fur_gs_material);
		m_axe->SetAllModelMaterials(m_fur_gs_material);
		return;
	}

	m_big_cat->SetAllModelMaterials(m_standard_material);
	m_sphere->SetAllModelMaterials(m_standard_material);
	m_axe->SetAllModelMaterials(m_standard_material);
}


void Game::SwapModel()
{
	switch(m_model_type)
	{
	case BIG_CAT: m_current_model = m_big_cat.get();
		break;
	case SPHERE: m_current_model = m_sphere.get();
		break;
	case AXE: m_current_model = m_axe.get();
		m_default_material_on = true;//set it to useing fur then swap
		SwitchMaterials();//axe should not use fur unless switched to
		break;
	}
}


void Game::UpdateTextureInputs()
{
	auto texture_set = m_textures.find(m_texture_set);
	m_fur_gs_material->SetStandardTextures(texture_set->second.albedo,
		texture_set->second.roughness, texture_set->second.specular);

	auto alpha = m_alpha_textures.find(m_fur_alpha_texture);
	auto mask = m_mask_textures.find(m_fur_mask_texture);
	m_fur_gs_material->SetFurTextures(alpha->second, mask->second);
}


#pragma region Creation Functions
void Game::CreateLight()
{
	m_light.position = Vector3(0, -6, -540.0f);
	m_light.colour[0] = 1;
	m_light.colour[1] = 1;
	m_light.colour[2] = 1;
	m_light.intensity = 1.43;
}


void Game::CreateCamera(Renderer& _renderer)
{
	m_camera = std::make_unique<Camera>(Vector3(-3, -27, -138), Quaternion(Vector3::Forward, TMath::Radians(0)),
		Vector3::One, TMath::Radians(70), _renderer.GetViewportAspectRatio(), 5, 500);
}


void Game::CreateModels(Renderer& _renderer)
{
	m_big_cat = std::make_unique<Model>();
	m_big_cat->LoadModel("./Big_Cat_Optimised.obj", _renderer);

	m_sphere = std::make_unique<Model>();
	m_sphere->LoadModel("./Sphere.obj", _renderer);

	m_axe = std::make_unique<Model>();
	m_axe->LoadModel("./Axe.obj", _renderer);

	m_big_cat->SetAllModelMaterials(m_fur_gs_material);
	m_sphere->SetAllModelMaterials(m_fur_gs_material);
	m_axe->SetAllModelMaterials(m_fur_gs_material);
	m_current_model = m_big_cat.get();
}


void Game::LoadTextures(Renderer& _renderer)
{
	LoadStandardTextures(_renderer);
	LoadAlphaTextures(_renderer);
	LoadMaskTextures(_renderer);	
}


void Game::LoadTextureSet(Renderer& _renderer, TextureSets _set_identifier,
	std::string _albedo_path, std::string _roughness_path, std::string _specular_path)
{
	TextureSet textures;
	textures.albedo = new Texture(_renderer, _albedo_path);
	textures.roughness = new Texture(_renderer, _roughness_path);
	textures.specular = new Texture(_renderer, _specular_path);
	m_textures.insert(std::pair<TextureSets, TextureSet>(_set_identifier, std::move(textures)));
}


void Game::LoadStandardTextures(Renderer& _renderer)
{
	LoadTextureSet(_renderer, BIG_CAT_TEX, "./Big_Cat_Optimised_Albedo.png", "./Grey.png", "./Big_Cat_Optimised_Specular.png");
	LoadTextureSet(_renderer, GIRAFFE_TEX, "./Giraffe_Albedo.png", "./White.png", "./White.png");
	LoadTextureSet(_renderer, NO_TEX, "./White.png", "./White.png", "./White.png");
}


void Game::LoadAlphaTextures(Renderer& _renderer)
{
	m_alpha_textures.insert(std::pair<FurTextureAlphas, Texture*>(CHUNKY_ALPHA, new Texture(_renderer, "./Fur_Alpha.png")));
	m_alpha_textures.insert(std::pair<FurTextureAlphas, Texture*>(FINE_APHA, new Texture(_renderer, "./Fur_Alpha_Fine.png")));
}


void Game::LoadMaskTextures(Renderer& _renderer)
{
	m_mask_textures.insert(std::pair<FurTextureMasks, Texture*>(BIG_CAT_MASK, new Texture(_renderer, "./Big_Cat_Optimised_Mask.png")));
	m_mask_textures.insert(std::pair<FurTextureMasks, Texture*>(GIRAFFE_MASK, new Texture(_renderer, "./Giraffe_Mask.png")));
	m_mask_textures.insert(std::pair<FurTextureMasks, Texture*>(NO_MASK, new Texture(_renderer, "./White.png")));
}


void Game::CreateGameData(InputManager& _input)
{
	m_game_data = std::make_unique<GameData>();
	m_game_data->input = &_input;
}


void Game::SetKeyBindings() const
{
	m_game_data->input->BindKey(FORWARD, "W");
	m_game_data->input->BindKey(BACKWARD, "S");
	m_game_data->input->BindKey(LEFT, "A");
	m_game_data->input->BindKey(RIGHT, "D");
	m_game_data->input->BindKey(UP, "E");
	m_game_data->input->BindKey(DOWN, "Q");
	m_game_data->input->BindKey(QUIT, "X");//oversight, can't bind escape
	m_game_data->input->BindKey(SWITCH, "F");
}


void Game::CreateMaterials(Renderer& _renderer)
{
	CreateStandardMaterial(_renderer);
	CreateFurMaterial(_renderer);
}


void Game::CreateStandardMaterial(Renderer& _renderer)
{
	m_standard_material = new Material();
	m_standard_material->CreateShaders("Standard_Material_Vertex_Shader.cso",
		"Standard_Material_Pixel_Shader.cso", _renderer);

	material_params.roughness = 1;
	material_params.specular = 0.3;
	material_params.diffuse[0] = 1;
	material_params.diffuse[1] = 1;
	material_params.diffuse[2] = 1;
}


void Game::CreateFurMaterial(Renderer& _renderer)
{
	LoadTextures(_renderer);
	auto start_tex = m_textures.find(BIG_CAT_TEX);
	auto start_mask = m_mask_textures.find(BIG_CAT_MASK);
	auto start_alpha = m_alpha_textures.find(CHUNKY_ALPHA);

	m_fur_gs_material = new FurMaterial();
	m_fur_gs_material->CreateShaders("Fur_Vertex_Shader.cso", "Fur_Pixel_Shader.cso", _renderer, "Fur_Shell_Geometry_Shader.cso");
	m_fur_gs_material->SetStandardTextures(start_tex->second.albedo, start_tex->second.roughness, start_tex->second.specular);
	m_fur_gs_material->SetFurTextures(start_alpha->second, start_mask->second);
	m_fur_gs_material->SetFurParameters(m_fur_parameters);

	m_fur_parameters.fur_mask_multiplier = 1;
	m_fur_parameters.base_clip = 0.71f;
	m_fur_parameters.shadow_intensity = 0.95f;
	m_fur_parameters.max_fur_length = 4;
	m_fur_parameters.layer_count = 16;
	m_fur_parameters.gravity = Vector3::Zero;
}


#pragma endregion


void Game::Tick()
{
	if (m_game_data->input->GetGameAction(GameAction::QUIT, InputManager::PRESSED))
		exit(0);

	m_game_data->delta_time = CalculateDeltaTime();

	m_camera->Tick(*m_game_data.get());
	m_current_model->Tick(*m_game_data.get());

	UpdateMaterialsParameters();

}


void Game::Draw(Renderer& _renderer) const
{
	UpdateRenderData(_renderer, *m_camera);
	m_current_model->Draw(_renderer);
}


void Game::UpdateRenderData(Renderer& _renderer, Camera& _camera) const
{
	_renderer.GetRenderData()->camera_view_matrix = _camera.GetViewMatrix();
	_renderer.GetRenderData()->camera_projection_matrix = _camera.GetProjectionMatrix();
	_renderer.GetRenderData()->camera_pos = _camera.GetTransform().GetPosition();
	_renderer.GetRenderData()->light = m_light;
}


void Game::UpdateMaterialsParameters() const
{
	m_standard_material->SetMaterialParams(material_params);
	m_fur_gs_material->SetMaterialParams(material_params);
	m_fur_gs_material->SetFurParameters(m_fur_parameters);
}


float Game::CalculateDeltaTime()
{
	DWORD currentTime = GetTickCount();
	float  dt = min(static_cast<float>((currentTime - m_playTime) / 1000.0f), 0.1f);
	m_playTime = currentTime;
	return dt;
}