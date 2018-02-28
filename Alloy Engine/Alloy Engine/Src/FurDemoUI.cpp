#include "FurDemoUI.h"
#include "Game.h"


FurDemoUI::FurDemoUI(Game& _game)
{
	m_game = &_game;
	CreateDebugUI();
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


void FurDemoUI::CreateDebugUI()
{
	CreateFurBar();
	CreateLightBar();
	CreateModelBar();
	CreateTextureBar();
	//CreateHelpBar();
}


void FurDemoUI::CreateFurBar()
{
	m_fur_bar = TwNewBar("Fur_Shader_Prototype");
	TwDefine("Fur_Shader_Prototype label='Fur Shader Parameters' ");
	TwDefine("Fur_Shader_Prototype color='46 53 49' text=light ");
	TwDefine("Fur_Shader_Prototype alpha=255");
	TwDefine("Fur_Shader_Prototype position= '20 20'");
	TwDefine("Fur_Shader_Prototype size='250 300'");
	CreateFurBarElements();
}


void FurDemoUI::CreateFurBarElements() const
{
	TwAddVarRW(m_fur_bar, "Diffuse", TW_TYPE_COLOR3F, &m_game->material_params.diffuse, " colormode=hls ");
	TwAddVarRW(m_fur_bar, "Rough", TW_TYPE_FLOAT, &m_game->material_params.roughness, "");
	TwAddVarRW(m_fur_bar, "Specular", TW_TYPE_FLOAT, &m_game->material_params.specular, "");

	TwDefine("Fur_Shader_Prototype/Rough  min=0.1 max=1 ");
	TwDefine("Fur_Shader_Prototype/Specular  min=0.1 max=1 ");

	TwDefine("Fur_Shader_Prototype/Rough   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Specular   step=0.1 ");

	TwAddVarRW(m_fur_bar, "Max_Length", TW_TYPE_FLOAT, &m_game->m_fur_parameters.max_fur_length, "");
	TwAddVarRW(m_fur_bar, "Layer_Count", TW_TYPE_INT8, &m_game->m_fur_parameters.layer_count, "");
	TwAddVarRW(m_fur_bar, "Base_Clip", TW_TYPE_FLOAT, &m_game->m_fur_parameters.base_clip, "");
	TwAddVarRW(m_fur_bar, "Shadowing", TW_TYPE_FLOAT, &m_game->m_fur_parameters.shadow_intensity, "");

	TwDefine("Fur_Shader_Prototype/Mask_Multiplier   step=0.1 ");
	TwDefine("Fur_Shader_Prototype/Max_Length   step=0.1 min=0.1");
	TwDefine("Fur_Shader_Prototype/Layer_Count   step=1 min=0 max= 32");
	TwDefine("Fur_Shader_Prototype/Base_Clip   step=0.01");
	TwDefine("Fur_Shader_Prototype/Shadowing   step=0.01 min=0 max= 1");

	TwAddVarRW(m_fur_bar, "Gravity_X", TW_TYPE_FLOAT, &m_game->m_fur_parameters.gravity.x, "");
	TwAddVarRW(m_fur_bar, "Gravity_Y", TW_TYPE_FLOAT, &m_game->m_fur_parameters.gravity.y, "");
	TwAddVarRW(m_fur_bar, "Gravity_Z", TW_TYPE_FLOAT, &m_game->m_fur_parameters.gravity.z, "");

	TwDefine("Fur_Shader_Prototype/Gravity_X   group=Gravity_Direction");
	TwDefine("Fur_Shader_Prototype/Gravity_Y   group=Gravity_Direction");
	TwDefine("Fur_Shader_Prototype/Gravity_Z   group=Gravity_Direction");

	TwAddButton(m_fur_bar, "Toggle_Fur", TWSwitchMaterials, static_cast<void*>(m_game), " label='Toggle Fur Shader'");
}


void FurDemoUI::CreateLightBar()
{
	m_light_bar = TwNewBar("Light");
	TwDefine("Light label='Light' ");
	TwDefine("Light label='Directional Light' ");
	TwDefine("Light color='46 53 49' text=light ");
	TwDefine("Light alpha=255");
	TwDefine("Light size='250 200'");
	TwDefine("Light position= '20 340'");

	CreateLightBarElements();

}


void FurDemoUI::CreateLightBarElements() const
{
	TwAddVarRW(m_light_bar, "Light Colour", TW_TYPE_COLOR3F, &m_game->m_light.colour, " colormode=hls ");
	TwAddVarRW(m_light_bar, "light_intensity", TW_TYPE_FLOAT, &m_game->m_light.intensity, "");
	TwAddVarRW(m_light_bar, "light_X", TW_TYPE_FLOAT, &m_game->m_light.position.x, "");
	TwAddVarRW(m_light_bar, "light_Y", TW_TYPE_FLOAT, &m_game->m_light.position.y, "");
	TwAddVarRW(m_light_bar, "light_Z", TW_TYPE_FLOAT, &m_game->m_light.position.z, "");

	//move light position to group
	TwDefine("Light/light_X   group=Light_Position");
	TwDefine("Light/light_Y   group=Light_Position");
	TwDefine("Light/light_Z   group=Light_Position");
	TwDefine("Light/light_intensity   step=0.01 ");
}


void FurDemoUI::CreateModelBar()
{
	m_model_bar = TwNewBar("Model");
	TwDefine("Model label='Model Swapper' ");
	TwDefine("Model color='46 53 49' text=light ");
	TwDefine("Model alpha=255");
	TwDefine("Model size='250 150'");
	TwDefine("Model position= '20 560'");
	TwDefine("Model valueswidth=fit");

	CreateModelBarElements();
}


void FurDemoUI::CreateModelBarElements()
{
	TwEnumVal models[] = { {Game::BIG_CAT, "Big cat" },{Game::SPHERE, "Sphere" },{Game::AXE, "Axe" } };
	m_tw_model_type = TwDefineEnum("Models", models, 3);//create antweak enum
	TwAddVarRW(m_model_bar, "Model Type", m_tw_model_type, &m_game->m_model_type, nullptr);//add enum
	TwAddButton(m_model_bar, "Swap Model", TWSwapModel, static_cast<void*>(m_game), " label='Swap Model'");//create button
}


void FurDemoUI::CreateTextureBar()
{
	m_texture_bar = TwNewBar("Textures");
	TwDefine("Textures label='Texture Swapper' ");
	TwDefine("Textures color='46 53 49' text=light ");
	TwDefine("Textures alpha=255");
	TwDefine("Textures size='300 150'");
	TwDefine("Textures position= '280 100'");
	TwDefine("Textures valueswidth=fit");

	CreateTextureBarElements();
}


void FurDemoUI::CreateHelpBar()
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


void FurDemoUI::CreateTextureBarElements()
{
	TwEnumVal texture_sets[] = { {Game::BIG_CAT_TEX, "Big cat textures" },{Game::GIRAFFE_TEX, "Giraffe textures" },{Game::NO_TEX, "No textures" } };
	m_tw_texture_type = TwDefineEnum("Textures", texture_sets, 3);//create antweak enum
	TwAddVarRW(m_texture_bar, "Texture Set", m_tw_texture_type, &m_game->m_texture_set, nullptr);//add enum

	TwEnumVal texture_alphas[] = { {Game::CHUNKY_ALPHA, "Chunky" },{Game::FINE_APHA, "Fine" } };
	m_tw_fur_alpha_texture_type = TwDefineEnum("Alphas", texture_alphas, 2);//create antweak enum
	TwAddVarRW(m_texture_bar, "Strand Alpha", m_tw_fur_alpha_texture_type, &m_game->m_fur_alpha_texture, nullptr);//add enum

	TwEnumVal texture_masks[] = { {Game::BIG_CAT_MASK, "Big cat mask" },{Game::GIRAFFE_MASK, "Giraffe mask" },{ Game::STAR_MASK, "Star mask" }, {Game::NO_MASK, "No mask" } };
	m_tw_fur_mask_texture_type = TwDefineEnum("Masks", texture_masks, 4);//create antweak enum
	TwAddVarRW(m_texture_bar, "Fur Mask", m_tw_fur_mask_texture_type, &m_game->m_fur_mask_texture, nullptr);//add enum

	TwAddButton(m_texture_bar, "Update Textures", TWUpdateTextures, static_cast<void*>(m_game), " label='Update Textures'");//create button
}