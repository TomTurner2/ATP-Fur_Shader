#pragma once
#include "GameObject.h"
#include <vector>
#include "Mesh.h"


class Model : public GameObject
{
public:
	Model() = default;
	~Model() = default;

	void Tick(GameData& _game_data) override;
	void Draw(Renderer& _renderer) override;

	void LoadModel(std::string _model_name, Renderer& _renderer);
	void LoadAllModelMaterials(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer);
	void SetAllModelMaterials(Material* _material);
	void SetAllMaterialParams(PBRMaterialParams _pbr_params);

	Mesh* GetMeshByName(std::string _name);
	int GetVertexCount();

private:
	std::vector<Mesh> m_meshes;
};
