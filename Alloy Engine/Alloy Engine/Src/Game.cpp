#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
#include "TMath.h"
#include "Model.h"
#include <AntTweakBar.h>
#include "TextureSet.h"


Game::Game(Renderer& _renderer, InputManager& _input)
{
	CreateLight();
	CreateCamera(_renderer);
	CreateModel(_renderer);
	CreateGameData(_input);
	CreateDebugUI();
	UpdateVertexCount();
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
		SwitchMaterials();//axe should never use fur
		break;
	}

	UpdateVertexCount();
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
	m_light.position = Vector3(0, 6, -115.0f);
	m_light.colour[0] = 1;
	m_light.colour[1] = 1;
	m_light.colour[2] = 1;
	m_light.intensity = 1;
}


void Game::CreateCamera(Renderer& _renderer)
{
	m_camera = std::make_unique<Camera>(Vector3(-3, -27, -138), Quaternion(Vector3::Forward, TMath::Radians(0)),
		Vector3::One, TMath::Radians(70), _renderer.GetViewportAspectRatio(), 5, 500);
}


void Game::CreateModel(Renderer& _renderer)
{
	m_big_cat = std::make_unique<Model>();
	m_big_cat->LoadModel("./Big_Cat.obj", _renderer);

	m_sphere = std::make_unique<Model>();
	m_sphere->LoadModel("./Sphere.obj", _renderer);

	m_axe = std::make_unique<Model>();
	m_axe->LoadModel("./Axe.obj", _renderer);

	material_params.roughness = 1;
	material_params.specular = 1;
	material_params.diffuse[0] = 1; 
	material_params.diffuse[1] = 1;
	material_params.diffuse[2] = 1;

	m_standard_material = new Material();
	m_standard_material->CreateShaders("Standard_Material_Vertex_Shader.cso",
		"Standard_Material_Pixel_Shader.cso", _renderer);

	m_fur_parameters.fur_mask_multiplier = 1;
	m_fur_parameters.base_clip = 0.71f;
	m_fur_parameters.shadow_intensity = 0.95f;
	m_fur_parameters.max_fur_length = 4;
	m_fur_parameters.layer_count = 16;
	m_fur_parameters.gravity = Vector3::Zero;
	

	LoadTextureSets(_renderer);
	auto start_tex = m_textures.find(BIG_CAT_TEX);
	auto start_mask = m_mask_textures.find(BIG_CAT_MASK);
	auto start_alpha = m_alpha_textures.find(CHUNKY_ALPHA);


	m_fur_gs_material = new FurMaterial();
	m_fur_gs_material->CreateShaders("Fur_Vertex_Shader.cso", "Fur_Pixel_Shader.cso", _renderer, "Fur_Shell_Geometry_Shader.cso");
	m_fur_gs_material->SetStandardTextures(start_tex->second.albedo, start_tex->second.roughness, start_tex->second.specular);
	m_fur_gs_material->SetFurTextures(start_alpha->second, start_mask->second);
	m_fur_gs_material->SetFurParameters(m_fur_parameters);


	m_big_cat->SetAllModelMaterials(m_fur_gs_material);
	m_sphere->SetAllModelMaterials(m_fur_gs_material);
	m_axe->SetAllModelMaterials(m_fur_gs_material);
	m_current_model = m_big_cat.get();
}


void Game::LoadTextureSets(Renderer& _renderer)
{
	
	TextureSet big_cat_textures;
	big_cat_textures.albedo = new Texture(_renderer, "./Big_Cat_Albedo.png");
	big_cat_textures.roughness = new Texture(_renderer, "./Big_Cat_Roughness.png");
	big_cat_textures.specular = new Texture(_renderer, "./Big_Cat_Specular.png");

	m_textures.insert(std::pair<TextureSets, TextureSet>(BIG_CAT_TEX, std::move(big_cat_textures)));

	TextureSet giraffe_textures;
	giraffe_textures.albedo = new Texture(_renderer, "./Giraffe_Albedo.png");
	giraffe_textures.roughness = new Texture(_renderer, "./White.png");
	giraffe_textures.specular = new Texture(_renderer, "./White.png");
	m_textures.insert(std::pair<TextureSets, TextureSet>(GIRAFFE_TEX, std::move(giraffe_textures)));

	TextureSet no_textures;
	no_textures.albedo = new Texture(_renderer, "./White.png");
	no_textures.roughness = new Texture(_renderer, "./White.png");
	no_textures.specular = new Texture(_renderer, "./White.png");
	m_textures.insert(std::pair<TextureSets, TextureSet>(NO_TEX, std::move(no_textures)));


	m_alpha_textures.insert(std::pair<FurTextureAlphas, Texture*>(CHUNKY_ALPHA, new Texture(_renderer, "./Fur_Alpha.png")));
	m_alpha_textures.insert(std::pair<FurTextureAlphas, Texture*>(FINE_APHA, new Texture(_renderer, "./Fur_Alpha_Fine.png")));

	m_mask_textures.insert(std::pair<FurTextureMasks, Texture*>(BIG_CAT_MASK, new Texture(_renderer, "./Big_Cat_Fur_Mask.png")));
	m_mask_textures.insert(std::pair<FurTextureMasks, Texture*>(GIRAFFE_MASK, new Texture(_renderer, "./Giraffe_Mask.png")));
	m_mask_textures.insert(std::pair<FurTextureMasks, Texture*>(NO_MASK, new Texture(_renderer, "./White.png")));
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
	m_game_data->input->BindKey(QUIT, "X");//oversight, can't bind escape
	m_game_data->input->BindKey(SWITCH, "F");
}


#pragma endregion


void Game::Tick()
{
	if (m_game_data->input->GetGameAction(GameAction::QUIT, InputManager::PRESSED))
		exit(0);

	m_game_data->delta_time = CalculateDeltaTime();
	m_camera->Tick(*m_game_data.get());
	m_current_model->Tick(*m_game_data.get());
	m_standard_material->SetMaterialParams(material_params);
	m_fur_gs_material->SetMaterialParams(material_params);
	m_fur_gs_material->SetFurParameters(m_fur_parameters);
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


float Game::CalculateDeltaTime()
{
	DWORD currentTime = GetTickCount();
	float  dt = min(static_cast<float>((currentTime - m_playTime) / 1000.0f), 0.1f);
	m_playTime = currentTime;
	return dt;
}


#pragma region UI Creation


void Game::CreateDebugUI()
{
	CreateFurBar();
	CreateFurBarElements();

	CreateLightBar();
	CreateLightBarElements();

	CreateModelBar();
	CreateModelBarElements();

	CreateTextureBar();
	CreateTextureBarElements();

	CreateHelpBar();//only element also added
}


void TW_CALL TWSwitchMaterials(void* _game)
{
	auto temp = static_cast<Game*>(_game);
	temp->SwitchMaterials();
}


void TW_CALL TWSwapModel(void* _game)
{
	auto temp = static_cast<Game*>(_game);
	temp->SwapModel();
}


void TW_CALL TWUpdateTextures(void* _game)
{
	auto temp = static_cast<Game*>(_game);
	temp->UpdateTextureInputs();
}


void TW_CALL TWUpdateStrandAlpha(void* _game)
{
	auto temp = static_cast<Game*>(_game);
	temp->UpdateTextureInputs();
}


void Game::CreateFurBar()
{
	m_fur_bar = TwNewBar("Fur_Shader_Prototype");
	TwDefine("Fur_Shader_Prototype label='Fur Shader Parameters' ");
	TwDefine("Fur_Shader_Prototype color='46 53 49' text=light ");
	TwDefine("Fur_Shader_Prototype alpha=255");
	TwDefine("Fur_Shader_Prototype position= '20 20'");
	TwDefine("Fur_Shader_Prototype size='250 300'");
}


void Game::CreateFurBarElements()
{
	TwAddVarRW(m_fur_bar, "Diffuse", TW_TYPE_COLOR3F, &material_params.diffuse, " colormode=hls ");
	TwAddVarRW(m_fur_bar, "Rough", TW_TYPE_FLOAT, &material_params.roughness, "");
	TwAddVarRW(m_fur_bar, "Specular", TW_TYPE_FLOAT, &material_params.specular, "");

	TwDefine("Fur_Shader_Prototype/Rough  min=0.1 max=1 ");
	TwDefine("Fur_Shader_Prototype/Specular  min=0.1 max=1 ");

	TwDefine("Fur_Shader_Prototype/Rough   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Specular   step=0.1 ");

	TwAddVarRW(m_fur_bar, "Max_Length", TW_TYPE_FLOAT, &m_fur_parameters.max_fur_length, "");
	TwAddVarRW(m_fur_bar, "Layer_Count", TW_TYPE_INT8, &m_fur_parameters.layer_count, "");
	TwAddVarRW(m_fur_bar, "Base_Clip", TW_TYPE_FLOAT, &m_fur_parameters.base_clip, "");
	TwAddVarRW(m_fur_bar, "Shadowing", TW_TYPE_FLOAT, &m_fur_parameters.shadow_intensity, "");

	TwDefine("Fur_Shader_Prototype/Mask_Multiplier   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Max_Length   step=0.1 min=0.1");
	TwDefine("Fur_Shader_Prototype/Layer_Count   step=1 min=0 max= 32");
	TwDefine("Fur_Shader_Prototype/Base_Clip   step=0.01");
	TwDefine("Fur_Shader_Prototype/Shadowing   step=0.01 min=0 max= 1");

	TwAddVarRW(m_fur_bar, "Gravity_X", TW_TYPE_FLOAT, &m_fur_parameters.gravity.x, "");
	TwAddVarRW(m_fur_bar, "Gravity_Y", TW_TYPE_FLOAT, &m_fur_parameters.gravity.y, "");
	TwAddVarRW(m_fur_bar, "Gravity_Z", TW_TYPE_FLOAT, &m_fur_parameters.gravity.z, "");

	TwDefine("Fur_Shader_Prototype/Gravity_X   group=Gravity_Direction");
	TwDefine("Fur_Shader_Prototype/Gravity_Y   group=Gravity_Direction");
	TwDefine("Fur_Shader_Prototype/Gravity_Z   group=Gravity_Direction");

	TwAddButton(m_fur_bar, "Toggle_Fur", TWSwitchMaterials, static_cast<void*>(this), " label='Toggle Fur Shader'");
}


void Game::CreateLightBar()
{
	m_light_bar = TwNewBar("Light");
	TwDefine("Light label='Light' ");
	TwDefine("Light label='Directional Light' ");
	TwDefine("Light color='46 53 49' text=light ");
	TwDefine("Light alpha=255");
	TwDefine("Light size='250 200'");
	TwDefine("Light position= '20 340'");

}


void Game::CreateLightBarElements()
{
	TwAddVarRW(m_light_bar, "Light Colour", TW_TYPE_COLOR3F, &m_light.colour, " colormode=hls ");
	TwAddVarRW(m_light_bar, "light_intensity", TW_TYPE_FLOAT, &m_light.intensity, "");
	TwAddVarRW(m_light_bar, "light_X", TW_TYPE_FLOAT, &m_light.position.x, "");
	TwAddVarRW(m_light_bar, "light_Y", TW_TYPE_FLOAT, &m_light.position.y, "");
	TwAddVarRW(m_light_bar, "light_Z", TW_TYPE_FLOAT, &m_light.position.z, "");

	//move light position to group
	TwDefine("Light/light_X   group=Light_Position");
	TwDefine("Light/light_Y   group=Light_Position");
	TwDefine("Light/light_Z   group=Light_Position");
	TwDefine("Light/light_intensity   step=0.01 ");
}


void Game::CreateModelBar()
{
	m_model_bar = TwNewBar("Model");
	TwDefine("Model label='Model Swapper' ");
	TwDefine("Model color='46 53 49' text=light ");
	TwDefine("Model alpha=255");
	TwDefine("Model size='250 150'");
	TwDefine("Model position= '20 560'");
	TwDefine("Model valueswidth=fit");
}


void Game::CreateModelBarElements()
{
	TwEnumVal models[] = { { BIG_CAT, "Big cat (high poly)" },{ SPHERE, "Sphere" },{ AXE, "Axe (high poly)" } };
	m_tw_model_type = TwDefineEnum("Models", models, 3);//create antweak enum
	TwAddVarRW(m_model_bar, "Model Type", m_tw_model_type, &m_model_type, nullptr);//add enum
	TwAddButton(m_model_bar, "Swap Model", TWSwapModel, static_cast<void*>(this), " label='Swap Model'");//create button
}


void Game::CreateHelpBar()
{
	m_help_bar = TwNewBar("Help");
	TwDefine("Help label='Help' ");
	TwDefine("Help color='46 53 49' text=light ");
	TwDefine("Help alpha=255");
	TwDefine("Help size='300 50'");
	TwDefine("Help position= '280 20'");
	TwDefine("Help valueswidth=fit");

	TwAddButton(m_help_bar, "help", nullptr, nullptr, "label='Please press Alt + Enter to go full screen'");//best way to just display text
}


void Game::CreateTextureBar()
{
	m_texture_bar = TwNewBar("Textures");
	TwDefine("Textures label='Texture Swapper' ");
	TwDefine("Textures color='46 53 49' text=light ");
	TwDefine("Textures alpha=255");
	TwDefine("Textures size='250 150'");
	TwDefine("Textures position= '200 560'");
	TwDefine("Textures valueswidth=fit");
}


void Game::CreateTextureBarElements()
{
	TwEnumVal texture_sets[] = { { BIG_CAT_TEX, "Big cat textures" },{ GIRAFFE_TEX, "Giraffe textures" },{ NO_TEX, "No textures" } };
	m_tw_texture_type = TwDefineEnum("Textures", texture_sets, 3);//create antweak enum
	TwAddVarRW(m_texture_bar, "Texture Set", m_tw_texture_type, &m_texture_set, nullptr);//add enum

	TwEnumVal texture_alphas[] = { { CHUNKY_ALPHA, "Chunky" },{ FINE_APHA, "Fine" }};
	m_tw_fur_alpha_texture_type = TwDefineEnum("Alphas", texture_alphas, 2);//create antweak enum
	TwAddVarRW(m_texture_bar, "Strand Alpha", m_tw_fur_alpha_texture_type, &m_fur_alpha_texture, nullptr);//add enum


	TwEnumVal texture_masks[] = { { BIG_CAT_MASK, "Big cat mask" },{ GIRAFFE_MASK, "Giraffe mask" }, {NO_MASK, "No mask"} };
	m_tw_fur_mask_texture_type = TwDefineEnum("Masks", texture_masks, 3);//create antweak enum
	TwAddVarRW(m_texture_bar, "Fur Mask", m_tw_fur_mask_texture_type, &m_fur_mask_texture, nullptr);//add enum


	TwAddButton(m_texture_bar, "Update Textures", TWUpdateTextures, static_cast<void*>(this), " label='Update Textures'");//create button
}


void Game::UpdateVertexCount() const
{
	if (m_model_bar == nullptr)
		return;

	std::string vert_count = "label='Model Vertices: " + std::to_string(m_current_model->GetVertexCount()) + "'";
	TwAddButton(m_model_bar, "vert_count", nullptr, nullptr, vert_count.c_str());//best way to just display text

	std::string vert_fur_count = "label='Fur Vertices: " + std::to_string((m_current_model->GetVertexCount() * m_fur_parameters.layer_count) - m_current_model->GetVertexCount()) + "'";
	TwAddButton(m_model_bar, "vert_fur_count", nullptr, nullptr, vert_fur_count.c_str());//best way to just display 

	std::string vert_both = "label='Combined Vertices: " + std::to_string(m_current_model->GetVertexCount() * m_fur_parameters.layer_count) + "'";
	TwAddButton(m_model_bar, "vert_both_count", nullptr, nullptr, vert_both.c_str());//best way to just display text

}
#pragma endregion 