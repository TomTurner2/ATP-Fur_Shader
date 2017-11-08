#include "Model.h"
#include "Mesh.h"
#include <algorithm>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Model::Tick(GameData & _game_data)
{
	for (auto &mesh : m_meshes)
	{
		mesh.SetTransform(m_transform);
	}
}


void Model::Draw(Renderer& _renderer)
{
	for (auto &mesh : m_meshes)
	{
		mesh.Draw(_renderer);
	}
}


void Model::LoadModel(std::string _model_name, Renderer& _renderer) const
{
	const aiScene* scene = aiImportFile(_model_name.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr)
	{
		printf("[Model](LoadModel) Failed to load model file");
		return;
	}

	std::vector<Mesh> meshes;
	meshes.reserve(scene->mNumMeshes);

	for (unsigned int mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id)
	{
		aiMesh* mesh = scene->mMeshes[mesh_id];

		std::vector<Vertex3D> verts;
		verts.reserve(mesh->mNumVertices);

		for (unsigned int vert_id = 0; vert_id < mesh->mNumVertices; ++vert_id)
		{
			aiVector3D vert = mesh->mVertices[vert_id];
			aiColor4t<float>* colour = mesh->mColors[vert_id];

			verts.push_back
			(
				Vertex3D 
				{
					vert.x, vert.y, vert.z, colour->r, colour->g, colour->b
				}
			);
		}

		std::vector<unsigned int> indicies;
		indicies.reserve(mesh->mNumFaces * 3);
		for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
		{
			indicies.push_back(mesh->mFaces[faceIdx].mIndices[0]);
			indicies.push_back(mesh->mFaces[faceIdx].mIndices[1]);
			indicies.push_back(mesh->mFaces[faceIdx].mIndices[2]);
		}

		meshes.push_back(Mesh(scene->mMeshes[mesh_id]->mName.C_Str()));
		meshes.back().CreateMesh(verts, indicies, _renderer);
	}
}


void Model::LoadAllModelMaterials(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer)
{
	for (auto mesh : m_meshes)
	{
		mesh.GetMaterial()->CreateShaders(_vertex_shader, _pixel_shader, _renderer);
	}
}


Mesh* Model::GetMeshByName(std::string _name)
{
	std::vector<Mesh>::iterator it = std::find_if(m_meshes.begin(),
		m_meshes.end(), [&_name](const Mesh& mesh) { return _name == mesh.GetName(); });

	if (it != m_meshes.end())
		return it._Ptr;

	return nullptr;
}
