#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include <vector>


class Model : public GameObject
{
public:
	Model() = default;
	~Model() = default;

	void Draw(Renderer& _renderer) override;
	void LoadModel(std::string _model_name);
	Mesh* GetMeshByName(std::string _name);

private:
	std::vector<Mesh> meshes;
};
