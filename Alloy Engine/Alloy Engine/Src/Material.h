#pragma once
#include <d3d11.h>
#include <string>


class Renderer;

//class should be inherited from
class Material
{
public:
	Material() = default;
	~Material();

	void CreateShaders(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer);
	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D11InputLayout* GetInputLayout() const;

	//TODO: add virtual functions for setting cam matrix/pos stuff and generic shader params

private:
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
	ID3D11InputLayout* m_input_layout = nullptr;
};
