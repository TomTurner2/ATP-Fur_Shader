#pragma once
#include "GameData.h"
#include <memory>
#include "Camera.h"
#include "Model.h"
#include <AntTweakBar.h>
#include "Light.h"
#include "FurMaterial.h"

class Renderer;

class Game
{
public:
	explicit Game(Renderer& _renderer, InputManager& _input);
	~Game() = default;

	void Tick();
	void Draw(Renderer& _renderer) const;

private:
	DWORD m_playTime;

	std::unique_ptr<GameData> m_game_data { nullptr };
	std::unique_ptr<Model> m_model { nullptr };
	std::unique_ptr<Camera> m_camera { nullptr };
	PBRMaterialParams material_params;
	Material* m_standard_material{ nullptr };
	FurMaterial* m_fur_gs_material{ nullptr };
	Light m_light;
	TwBar* m_bar { nullptr };
	float m_model_rot = 0;
	bool m_default_material_on = true;

	void SwitchMaterials() const;
	void CreateLight();
	void CreateCamera(Renderer& _renderer);
	void CreateModel(Renderer& _renderer);
	void CreateGameData(InputManager& _input);
	void CreateDebugUI();
	void BindParamsToUI();

	void UpdateRenderData(Renderer& _renderer, Camera& _camera) const;
	float CalculateDeltaTime();
};
