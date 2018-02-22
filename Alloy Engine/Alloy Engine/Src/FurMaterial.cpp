#include "FurMaterial.h"
#include "Renderer.h"


void FurMaterial::LoadFurTextures(Renderer& _renderer, std::string _fur_mask_path, std::string _fur_alpha_path)
{
	m_fur_alpha = new Texture(_renderer, _fur_alpha_path);
	m_fur_mask = new Texture(_renderer, _fur_mask_path);
}


void FurMaterial::SetFurParameters(FurParameters _fur_params)
{
	m_fur_params = _fur_params;
}


void FurMaterial::CreateBuffers(Renderer& _renderer)
{
	Material::CreateBuffers(_renderer);//create base constant buffers

	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0x00;
	buffer_desc.StructureByteStride = 0x00;

	HRESULT hr = {};

	buffer_desc.ByteWidth = sizeof(m_fur_params);
	hr = _renderer.GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_fur_param_buffer);
	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](CreateBuffers) Failed to create fur param buffer", "Error", MB_OK);
	}
}


//remember to take into account base constant buffer registers when writing shaders
void FurMaterial::UpdateAndAddCustomBuffers(Renderer& _renderer)
{
	HRESULT hr{ };
	D3D11_MAPPED_SUBRESOURCE vs_per_object_mapping{ nullptr };
	hr = _renderer.GetDeviceContext()->Map(m_fur_param_buffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &vs_per_object_mapping);//map across buffer

	memcpy(vs_per_object_mapping.pData, &m_fur_params, sizeof(FurParameters));
	_renderer.GetDeviceContext()->Unmap(m_fur_param_buffer, 0);

	if (hr != MB_OK)
	{
		MessageBox(nullptr, "[Material](UpdateBuffers) Failed to update buffer", "Error", MB_OK);
	}


	m_ps_buffers.push_back(m_fur_param_buffer);
	m_gs_buffers.push_back(m_fur_param_buffer);

	m_ps_resources.push_back(m_fur_alpha->GetTexture());
	m_ps_resources.push_back(m_fur_mask->GetTexture());

	m_gs_resources.push_back(m_fur_alpha->GetTexture());
	m_gs_resources.push_back(m_fur_mask->GetTexture());
}
