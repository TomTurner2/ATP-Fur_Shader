#pragma once
#include "GameObject.h"
#include "Vertex.h"
#include <d3d11.h>
#include "Material.h"
#include <vector>


class Renderer;


class Mesh : public GameObject
{
public:
	Mesh(std::string _name);
	~Mesh();

	std::string GetName() const;
	Material* GetMaterial() const;

	void CreateMesh(const std::vector<Vertex3D>& _vertices, const std::vector<unsigned int>& _indicies, Renderer& _renderer);
	void Draw(Renderer& _renderer) override;

private:
	std::string m_name = "";
	Material* m_material { nullptr };
	ID3D11Buffer* m_vertex_buffer { nullptr };
	ID3D11Buffer* m_index_buffer{ nullptr };
	unsigned int m_indice_count = 0;
	unsigned int m_vertex_count = 0;
};
