#pragma once
#include "GameData.h"
#include <memory>
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "FurMaterial.h"
#include "TextureSet.h"
#include "FurDemoUI.h"


class Renderer;


class Game
{
public:
	explicit Game(Renderer& _renderer, InputManager& _input);
	~Game() = default;

	void Tick();
	void Draw(Renderer& _renderer) const;

	void SwitchMaterials();//called by UI
	void SwapModel();
	void UpdateTextureInputs();

private:
	DWORD m_playTime;

	std::unique_ptr<GameData> m_game_data { nullptr };
	std::unique_ptr<Camera> m_camera{ nullptr };
	std::unique_ptr<Model> m_big_cat { nullptr };
	std::unique_ptr<Model> m_sphere { nullptr };
	std::unique_ptr<Model> m_axe{ nullptr };

	friend class FurDemoUI;//not good practice but this is a protoype
	std::unique_ptr<FurDemoUI> m_demo_ui{ nullptr };	

	enum Models : int
	{
		BIG_CAT,
		SPHERE,
		AXE
	} m_model_type = BIG_CAT;
	Model* m_current_model { nullptr };

	enum TextureSets : int
	{
		BIG_CAT_TEX,
		TIGER_TEX,
		NO_TEX
	} m_texture_set = BIG_CAT_TEX;
	std::map<TextureSets, TextureSet> m_textures;

	enum FurTextureAlphas : int
	{
		CHUNKY_ALPHA,
		FINE_APHA
	} m_fur_alpha_texture = CHUNKY_ALPHA;
	std::map<FurTextureAlphas, Texture*> m_alpha_textures;

	enum FurTextureMasks : int
	{
		BIG_CAT_MASK,
		TIGER_MASK,
		STAR_MASK,
		NO_MASK
	} m_fur_mask_texture = BIG_CAT_MASK;
	std::map<FurTextureMasks, Texture*> m_mask_textures;


	PBRMaterialParams material_params;
	Material* m_standard_material { nullptr };
	FurMaterial* m_fur_gs_material { nullptr };
	FurParameters m_fur_parameters;
	Light m_light;

	bool m_default_material_on = true;

	void CreateLight();
	void CreateCamera(Renderer& _renderer);
	void CreateModels(Renderer& _renderer);
	void CreateGameData(InputManager& _input);
	void CreateMaterials(Renderer& _renderer);
	void CreateStandardMaterial(Renderer& _renderer);
	void CreateFurMaterial(Renderer& _renderer);

	void LoadTextures(Renderer& _renderer);
	void LoadTextureSet(Renderer& _renderer, TextureSets _set_identifier,
		std::string _albedo_path, std::string _roughness_path, std::string _specular_path);
	void LoadStandardTextures(Renderer& _renderer);
	void LoadAlphaTextures(Renderer& _renderer);
	void LoadMaskTextures(Renderer& _renderer);

	void SetKeyBindings() const;
	void UpdateRenderData(Renderer& _renderer, Camera& _camera) const;
	void UpdateMaterialsParameters() const;
	float CalculateDeltaTime();
};
