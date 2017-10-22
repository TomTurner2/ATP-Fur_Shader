#pragma once
#include <d3d11.h>

class Renderer;

class Triangle
{
public:
	Triangle(Renderer& _renderer);
	~Triangle();

	void Draw(Renderer& _renderer) const;
private:
	void CreateMesh(Renderer& _renderer);
	void CreateShaders(Renderer& _renderer);

	ID3D11Buffer* m_vertex_buffer = nullptr;
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
	ID3D11InputLayout* m_input_layout = nullptr;
};
