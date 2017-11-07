#include "Material.h"
#include <vector>
#include "Renderer.h"
#include <fstream>
#include "Macros.h"


Material::~Material()
{
	SAFE_RELEASE(m_vertex_shader);
	SAFE_RELEASE(m_pixel_shader);
	SAFE_RELEASE(m_input_layout);
}


void Material::CreateShaders(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer)
{
	std::ifstream vs_file(_vertex_shader, std::ios::binary);
	std::ifstream ps_file(_pixel_shader, std::ios::binary);

	// Load file data
	std::vector<char> vs_data = { std::istreambuf_iterator<char>(vs_file), std::istreambuf_iterator<char>() };
	std::vector<char> ps_data = { std::istreambuf_iterator<char>(ps_file), std::istreambuf_iterator<char>() };

	_renderer.GetDevice()->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &m_vertex_shader);
	_renderer.GetDevice()->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &m_pixel_shader);

	// Create input layouts
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }// Append is offset
	};

	auto result = _renderer.GetDevice()->CreateInputLayout(layout, sizeof(layout), vs_data.data(),
		vs_data.size(), &m_input_layout);

	if (result == S_OK)
		return;

	MessageBox(nullptr, "[Material](CreateShaders) Failed to create shader input layout", "Error", MB_OK);
}


ID3D11VertexShader* Material::GetVertexShader() const
{
	return m_vertex_shader;
}


ID3D11PixelShader* Material::GetPixelShader() const
{
	return m_pixel_shader;
}


ID3D11InputLayout* Material::GetInputLayout() const
{
	return m_input_layout;
}