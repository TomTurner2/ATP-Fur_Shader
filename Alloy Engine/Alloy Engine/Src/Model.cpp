#include "Model.h"
#include "Mesh.h"
#include <algorithm>


void Model::Draw(Renderer& _renderer)
{
	for (auto &mesh : meshes)
	{
		mesh.Draw(_renderer);
	}
}


void Model::LoadModel(std::string _model_name)
{
	//TODO: use assimp to create meshes from imported file
}


Mesh* Model::GetMeshByName(std::string _name)
{
	std::vector<Mesh>::iterator it = std::find_if(meshes.begin(),
		meshes.end(), [&_name](const Mesh& mesh) { return _name == mesh.GetName(); });

	if (it != meshes.end())
		return it._Ptr;

	return nullptr;
}
