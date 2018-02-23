#pragma once
#include "GameData.h"
#include <memory>
#include "Camera.h"
#include "Model.h"
#include <AntTweakBar.h>
#include "Light.h"
#include "FurMaterial.h"
#include "TextureSet.h"


class Renderer;

class Game
{
public:
	explicit Game(Renderer& _renderer, InputManager& _input);
	~Game() = default;

	void Tick();
	void Draw(Renderer& _renderer) const;

	void SwitchMaterials();//Accessible by anttweak
	void SwapModel();
	void UpdateTextureInputs();


private:
	DWORD m_playTime;

	std::unique_ptr<GameData> m_game_data { nullptr };
	std::unique_ptr<Camera> m_camera{ nullptr };
	std::unique_ptr<Model> m_big_cat { nullptr };
	std::unique_ptr<Model> m_sphere { nullptr };
	std::unique_ptr<Model> m_axe{ nullptr };

	enum Models : int//enum must be in sim class so AntTweak function can access it
	{
		BIG_CAT,
		SPHERE,
		AXE
	};


	TwType m_tw_model_type;
	Models m_model_type = BIG_CAT;
	Model* m_current_model { nullptr };


	enum TextureSets : int//enum must be in sim class so AntTweak function can access it
	{
		BIG_CAT_TEX,
		GIRAFFE_TEX,
		NO_TEX
	};

	TwType m_tw_texture_type;
	TextureSets m_texture_set = BIG_CAT_TEX;
	std::map<TextureSets, TextureSet> m_textures;

	enum FurTextureAlphas : int//enum must be in sim class so AntTweak function can access it
	{
		CHUNKY_ALPHA,
		FINE_APHA
	};

	TwType m_tw_fur_alpha_texture_type;
	FurTextureAlphas m_fur_alpha_texture = CHUNKY_ALPHA;
	std::map<FurTextureAlphas, Texture*> m_alpha_textures;

	enum FurTextureMasks : int//enum must be in sim class so AntTweak function can access it
	{
		BIG_CAT_MASK,
		GIRAFFE_MASK,
		NO_MASK
	};

	TwType m_tw_fur_mask_texture_type;
	FurTextureMasks m_fur_mask_texture = BIG_CAT_MASK;
	std::map<FurTextureMasks, Texture*> m_mask_textures;


	PBRMaterialParams material_params;
	Material* m_standard_material { nullptr };
	FurMaterial* m_fur_gs_material { nullptr };
	FurParameters m_fur_parameters;
	Light m_light;

	TwBar* m_fur_bar { nullptr };
	TwBar* m_light_bar { nullptr };
	TwBar* m_model_bar { nullptr };
	TwBar* m_texture_bar{ nullptr };
	TwBar* m_help_bar { nullptr };

	bool m_default_material_on = true;
	int m_current_vertex_count = 0;
	int m_current_fur_vertex_count = 0;

	void CreateLight();
	void CreateCamera(Renderer& _renderer);
	void CreateModel(Renderer& _renderer);
	void CreateGameData(InputManager& _input);
	void LoadTextureSets(Renderer& _renderer);

	void CreateDebugUI();
	void CreateFurBar();
	void CreateFurBarElements();
	void CreateLightBar();
	void CreateLightBarElements();
	void CreateModelBar();
	void CreateModelBarElements();
	void CreateHelpBar();
	void CreateTextureBar();
	void CreateTextureBarElements();
	void UpdateVertexCount() const;

	void UpdateRenderData(Renderer& _renderer, Camera& _camera) const;
	float CalculateDeltaTime();
};
