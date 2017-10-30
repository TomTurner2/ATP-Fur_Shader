#include "Triangle.h"
#include "Renderer.h"
#include <fstream>
#include <vector>
#include <iterator>


#define SAFE_RELEASE(r) {if (r) { (r)->Release(); (r) = nullptr; } }


struct Vertex
{
	float x, y;
	float r, g, b;
};


Triangle::Triangle(Renderer& _renderer)
{
	CreateMesh(_renderer);
	CreateShaders(_renderer);
}


Triangle::~Triangle()
{
	SAFE_RELEASE(m_vertex_buffer);
	SAFE_RELEASE(m_vertex_shader);
	SAFE_RELEASE(m_pixel_shader);
	SAFE_RELEASE(m_input_layout);
}


void Triangle::Draw(Renderer& _renderer) const
{
	auto device_context = _renderer.getDeviceContext();

	// Bind shaders
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->IASetInputLayout(m_input_layout);
	device_context->VSSetShader(m_vertex_shader, nullptr, 0);
	device_context->PSSetShader(m_pixel_shader, nullptr, 0);

	// Bind vertex buffer
	UINT stride = sizeof(Vertex);// How far the VB must move to access the next vertex
	UINT offset = 0;// Offset between each structure (used to skip info)

	device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	device_context->Draw(3, 0);// Draw call (3 is the vertex count)
}


void Triangle::CreateMesh(Renderer& _renderer)
{
	// Create Vertices
	Vertex vertices[]
	{
		{ -1, -1, 1, 0, 0 },
		{ 0, 1, 0, 1, 0 },
		{ 1, -1, 0, 0, 1 }
	};

	// Create Vertex Buffer (passed into GPU VRAM)
	auto vertex_buffer_desc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vertex_data = { nullptr };
	vertex_data.pSysMem = vertices;

	auto result = _renderer.getDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);

	if (result == S_OK)
		return;

	MessageBox(nullptr, "[Triangle](CreateMesh) Failed to create vertex buffer", "Error", MB_OK);
	exit(0);
}


void Triangle::CreateShaders(Renderer& _renderer)
{
	std::ifstream vs_file("Triangle_Vertex_Shader.cso", std::ios::binary);
	std::ifstream ps_file("Triangle_Pixel_Shader.cso", std::ios::binary);

	// Load file data
	std::vector<char> vs_data = { std::istreambuf_iterator<char>(vs_file), std::istreambuf_iterator<char>() };
	std::vector<char> ps_data = { std::istreambuf_iterator<char>(ps_file), std::istreambuf_iterator<char>() };

	_renderer.getDevice()->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &m_vertex_shader);
	_renderer.getDevice()->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &m_pixel_shader);

	// Create input layouts
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }// Append is offset
	};

	auto result = _renderer.getDevice()->CreateInputLayout(layout, 2, vs_data.data(),
		vs_data.size(), &m_input_layout);

	if (result != S_OK)
		return;

	MessageBox(nullptr, "[Triangle](CreateShaders) Failed to create shader input layout", "Error", MB_OK);
	exit(0);
}
