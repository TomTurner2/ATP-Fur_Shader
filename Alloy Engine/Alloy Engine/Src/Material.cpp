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

	CreateBuffers(_renderer);

	if (result == S_OK)
		return;

	MessageBox(nullptr, "[Material](CreateShaders) Failed to create shader input layout", "Error", MB_OK);
}


void Material::CreateBuffers(Renderer& _renderer)
{
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0x00;
	buffer_desc.StructureByteStride = 0x00;

	HRESULT hr = {};

	buffer_desc.ByteWidth = sizeof(m_vs_per_object.second);
	hr = _renderer.GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_vs_per_object_buffer);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create buffer", "Error", MB_OK);
	}

	buffer_desc.ByteWidth = sizeof(m_vs_per_frame.second);
	hr = _renderer.GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_vs_per_frame_buffer);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create buffer", "Error", MB_OK);
	}


	buffer_desc.ByteWidth = sizeof(m_ps_per_frame.second);
	hr = _renderer.GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_ps_per_frame_buffer);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create buffer", "Error", MB_OK);
	}
}


void Material::UpdateBuffers(Renderer& _renderer)
{
	HRESULT hr = {};

	if (m_vs_per_object.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE vs_per_object_mapping;
		hr = _renderer.GetDeviceContext()->Map(m_vs_per_object_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0x00, &vs_per_object_mapping);//map across buffer

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
	}


	if (m_vs_per_frame.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE vs_per_frame_mapping;
		hr = _renderer.GetDeviceContext()->Map(m_vs_per_frame_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0x00, &vs_per_frame_mapping);

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
	}


	if (m_ps_per_frame.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE ps_per_frame_mapping;
		hr = _renderer.GetDeviceContext()->Map(m_ps_per_frame_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0x00, &ps_per_frame_mapping);

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
	}

	// pass buffers to pipeline
	ID3D11Buffer* vs_buffers[] = { m_vs_per_object_buffer, m_vs_per_frame_buffer };
	_renderer.GetDeviceContext()->VSSetConstantBuffers(0, _countof(vs_buffers), vs_buffers);//set both vs buffers

	_renderer.GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ps_per_frame_buffer);//currently I only have one ps buffer, need to add scene buffer containing lights
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


void Material::SetTransformMatrix(Matrix _transform)
{
	m_vs_per_object.second.transform = _transform;
	m_vs_per_object.first = true;
}


void Material::SetView(Matrix _view)
{
	m_vs_per_frame.second.view = _view;
	m_vs_per_frame.first = true;
}


void Material::SetProjection(Matrix _projection)
{
	m_vs_per_frame.second.proj = _projection;
	m_vs_per_frame.first = true;
}


void Material::SetCameraPosition(Vector3 _pos)
{
	m_ps_per_frame.second.camera_position = _pos;
	m_ps_per_frame.first = true;
}
