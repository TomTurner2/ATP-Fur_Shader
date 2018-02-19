#include "Material.h"
#include <vector>
#include "Renderer.h"
#include <fstream>
#include "Macros.h"
#include <stdlib.h>


Material::Material()
{
	m_vs_per_object.first = true;
	m_vs_per_frame.first = true;
	m_ps_per_frame.first = true;
	m_ps_per_scene.first = true;
	m_ps_per_object.first = true;
}


Material::~Material()
{
	SAFE_RELEASE(m_vs_per_frame_buffer)
	SAFE_RELEASE(m_vs_per_object_buffer)
	SAFE_RELEASE(m_vertex_shader);
	SAFE_RELEASE(m_ps_per_frame_buffer);
	SAFE_RELEASE(m_pixel_shader);
	SAFE_RELEASE(m_geometry_shader);
	SAFE_RELEASE(m_input_layout);
}


void Material::CreateShaders(std::string _vertex_shader, std::string _pixel_shader, Renderer& _renderer, std::string _geometry_shader)
{
	std::ifstream vs_file(_vertex_shader, std::ios::binary);
	std::ifstream ps_file(_pixel_shader, std::ios::binary);
	std::ifstream gs_file(_geometry_shader, std::ios::binary);

	// Load file data
	std::vector<char> vs_data = { std::istreambuf_iterator<char>(vs_file), std::istreambuf_iterator<char>() };
	std::vector<char> ps_data = { std::istreambuf_iterator<char>(ps_file), std::istreambuf_iterator<char>() };
	std::vector<char> gs_data = { std::istreambuf_iterator<char>(gs_file), std::istreambuf_iterator<char>() };
	HRESULT hr{};
	hr = _renderer.GetDevice()->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &m_vertex_shader);
	hr = _renderer.GetDevice()->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &m_pixel_shader);
	hr =_renderer.GetDevice()->CreateGeometryShader(gs_data.data(), gs_data.size(), nullptr, &m_geometry_shader);

	// Create input layouts
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int element_count = _countof(layout);

	auto result = _renderer.GetDevice()->CreateInputLayout(layout, element_count, vs_data.data(),
		vs_data.size(), &m_input_layout);

	CreateBuffers(_renderer);

	if (result == S_OK)
		return;

	MessageBox(nullptr, "[Material](CreateShaders) Failed to create shader input layout", "Error", MB_OK);
}


void Material::LoadStandardTextures(std::string _albedo_path, std::string _roughness_path, std::string _specular_path, Renderer & _renderer)
{
	m_albedo_texture = new Texture(_renderer, _albedo_path);
	m_rougness_texture = new Texture(_renderer, _roughness_path);
	m_specular_texture = new Texture(_renderer, _specular_path);
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

	buffer_desc.ByteWidth = sizeof(m_ps_per_scene.second);
	hr = _renderer.GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_ps_per_scene_buffer);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create buffer", "Error", MB_OK);
	}


	buffer_desc.ByteWidth = sizeof(m_ps_per_object.second);
	hr = _renderer.GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_ps_per_object_buffer);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create buffer", "Error", MB_OK);
	}

	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = _renderer.GetDevice()->CreateSamplerState(&sampler_desc, &m_sample_state);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create sampler state", "Error", MB_OK);
	}
}


void Material::UpdateBuffers(Renderer& _renderer)
{
	HRESULT hr = {};

	if (m_vs_per_object.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE vs_per_object_mapping { nullptr };
		hr = _renderer.GetDeviceContext()->Map(m_vs_per_object_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &vs_per_object_mapping);//map across buffer

		memcpy(vs_per_object_mapping.pData, &m_vs_per_object.second, sizeof(VSPerObjectBuffer));
		/*printf("VSPerObjectBuffer %d\n VSPerFrameBuffer %d\n PSPerFrameBuffer %d\n PSPerSceneBuffer %d\n PSPerObjectBuffer %d",
			sizeof(VSPerObjectBuffer), sizeof(VSPerFrameBuffer), sizeof(PSPerFrameBuffer), sizeof(PSPerSceneBuffer), sizeof(PSPerObjectBuffer));*/

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
		m_vs_per_object.first = false;//is not outdated
	}


	if (m_vs_per_frame.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE vs_per_frame_mapping { nullptr };
		hr = _renderer.GetDeviceContext()->Map(m_vs_per_frame_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &vs_per_frame_mapping);

		memcpy(vs_per_frame_mapping.pData, &m_vs_per_frame.second, sizeof(VSPerFrameBuffer));

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
		m_vs_per_frame.first = false;
	}


	if (m_ps_per_frame.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE ps_per_frame_mapping { nullptr };
		hr = _renderer.GetDeviceContext()->Map(m_ps_per_frame_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &ps_per_frame_mapping);

		memcpy(ps_per_frame_mapping.pData, &m_ps_per_frame.second, sizeof(PSPerFrameBuffer));

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
		m_ps_per_frame.first = false;
	}


	if (m_ps_per_scene.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE ps_per_scene_mapping{ nullptr };
		hr = _renderer.GetDeviceContext()->Map(m_ps_per_scene_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &ps_per_scene_mapping);

		memcpy(ps_per_scene_mapping.pData, &m_ps_per_scene.second, sizeof(PSPerSceneBuffer));

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
		m_ps_per_scene.first = false;
	}


	if (m_ps_per_object.first)//if outdated
	{
		D3D11_MAPPED_SUBRESOURCE ps_per_object_mapping{ nullptr };
		hr = _renderer.GetDeviceContext()->Map(m_ps_per_object_buffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &ps_per_object_mapping);

		memcpy(ps_per_object_mapping.pData, &m_ps_per_object.second, sizeof(PSPerObjectBuffer));

		if (hr != MB_OK)
		{
			MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
		}
		m_ps_per_object.first = false;
	}

	m_vs_buffers.clear();
	m_vs_buffers.reserve(5);
	m_vs_buffers.push_back(m_vs_per_object_buffer);
	m_vs_buffers.push_back(m_vs_per_frame_buffer);

	m_ps_buffers.clear();
	m_ps_buffers.reserve(5);
	m_ps_buffers.push_back(m_ps_per_frame_buffer);
	m_ps_buffers.push_back(m_ps_per_scene_buffer);
	m_ps_buffers.push_back(m_ps_per_object_buffer);

	m_gs_buffers.clear();
	m_gs_buffers.reserve(2);
	m_gs_buffers.push_back(m_vs_per_object_buffer);
	m_gs_buffers.push_back(m_vs_per_frame_buffer);

	m_ps_resources.clear();
	m_ps_resources.reserve(5);
	if (m_albedo_texture != nullptr)
		m_ps_resources.push_back(m_albedo_texture->GetTexture());
	if (m_rougness_texture != nullptr)
	m_ps_resources.push_back(m_rougness_texture->GetTexture());
	if (m_specular_texture != nullptr)
	m_ps_resources.push_back(m_specular_texture->GetTexture());
	
	m_gs_resources.clear();

	UpdateAndAddCustomBuffers(_renderer);
	SetBuffers(_renderer);
}


//map your buffers then Add any vs buffers to m_vs_buffers and add any ps buffers to m_ps_buffers
void Material::UpdateAndAddCustomBuffers(Renderer& _renderer)
{}


void Material::SetBuffers(Renderer& _renderer)
{
	if(m_vs_buffers.size() > 0)
		_renderer.GetDeviceContext()->VSSetConstantBuffers(0, m_vs_buffers.size(), &m_vs_buffers[0]);

	if(m_ps_buffers.size() > 0)
		_renderer.GetDeviceContext()->PSSetConstantBuffers(0, m_ps_buffers.size(), &m_ps_buffers[0]);

	if (m_gs_buffers.size() > 0)
		_renderer.GetDeviceContext()->GSSetConstantBuffers(0, m_gs_buffers.size(), &m_gs_buffers[0]);

	_renderer.GetDeviceContext()->PSSetSamplers(0, 1, &m_sample_state);

	if (m_ps_resources.size() > 0)
	_renderer.GetDeviceContext()->PSSetShaderResources(0, m_ps_resources.size(), &m_ps_resources[0]);

	if (m_gs_resources.size() > 0)
		_renderer.GetDeviceContext()->GSSetShaderResources(0, m_gs_resources.size(), &m_gs_resources[0]);
}


ID3D11VertexShader* Material::GetVertexShader() const
{
	return m_vertex_shader;
}


ID3D11PixelShader* Material::GetPixelShader() const
{
	return m_pixel_shader;
}


ID3D11GeometryShader * Material::GetGeometryShader() const
{
	return m_geometry_shader;
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
	if (m_ps_per_frame.second.camera_position == _pos)
		return;

	m_ps_per_frame.second.camera_position = _pos;
	m_ps_per_frame.first = true;
}


void Material::SetLight(Light _light)
{
	m_ps_per_scene.second.light = _light;
	m_ps_per_scene.first = true;
}


void Material::SetMaterialParams(PBRMaterialParams _pbr_params)
{
	m_ps_per_object.second.material_params = _pbr_params;
	m_ps_per_object.first = true;
}
