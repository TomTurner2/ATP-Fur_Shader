#include "Model.h"
#include "Mesh.h"
#include <algorithm>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Model::Tick(GameData& _game_data)
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


void Model::LoadModel(std::string _model_name, Renderer& _renderer)
{
	unsigned int process_flags =
		aiProcess_RemoveRedundantMaterials |
		aiProcess_FindInvalidData |
		0;

	const aiScene* scene = aiImportFile(_model_name.c_str(), process_flags);

	if (scene == nullptr)
	{
		printf("[Model](LoadModel) Failed to load model file\n" );
		printf(aiGetErrorString());
		return;
	}

	m_meshes.reserve(scene->mNumMeshes);

	for (unsigned int mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id)
	{
		aiMesh* mesh = scene->mMeshes[mesh_id];

		std::vector<Vertex3D> verts;
		verts.reserve(mesh->mNumVertices);

		for (unsigned int vert_id = 0; vert_id < mesh->mNumVertices; ++vert_id)
		{
			aiVector3D vert = mesh->mVertices[vert_id];
			Vector3 vertex_position = Vector3::Zero;
			vertex_position.x = vert.x;
			vertex_position.y = vert.y;
			vertex_position.z = vert.z;

			aiVector3D normal_in = mesh->mNormals[vert_id];
			Vector3 normal = Vector3::Zero;
			normal.x = normal_in.x;
			normal.y = -normal_in.y;
			normal.z = normal_in.z;

			Vertex3D vert_data = { vertex_position, 1, normal, 1, };
			verts.push_back(vert_data);
		}

		std::vector<unsigned int> indicies;
		indicies.reserve(mesh->mNumFaces * 3);

		for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; ++faceIdx)
		{
			for (unsigned int i = 0; i < mesh->mFaces[faceIdx].mNumIndices; ++i)
			{
				indicies.push_back(mesh->mFaces[faceIdx].mIndices[i]);
			}
		}

		m_meshes.push_back(Mesh(scene->mMeshes[mesh_id]->mName.C_Str()));
		m_meshes.back().CreateMesh(verts, indicies, _renderer);
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
