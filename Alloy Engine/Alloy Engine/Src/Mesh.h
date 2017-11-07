#pragma once
#include "GameObject.h"
#include "Vertex.h"
#include <d3d11.h>
#include "Material.h"


class Renderer;


class Mesh : GameObject
{
public:
	Mesh();
	~Mesh();

	std::string GetName() const;
	Material* GetMaterial() const;

	void CreateMesh(Vertex3D verticies[], Renderer& _renderer);
	void Draw(Renderer& _renderer) override;

private:
	std::string m_name = "";
	std::unique_ptr<Material> m_material = { nullptr };
	ID3D11Buffer* m_vertex_buffer = { nullptr };
};
