#pragma once
#include "GameObject.h"
#include <vector>
#include "Mesh.h"


class Model : public GameObject
{
public:
	Model() = default;
	~Model() = default;

	void Draw(Renderer& _renderer) override;
	void LoadModel(std::string _model_name, Renderer& _renderer);
	void LoadAllModelMaterials(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer);
	Mesh* GetMeshByName(std::string _name);

private:
	std::vector<Mesh> m_meshes;
};
