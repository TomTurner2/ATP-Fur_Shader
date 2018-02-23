#include "FurTexture.h"
#include "Renderer.h"
#include <random>


FurTexture::FurTexture(Renderer& _renderer, unsigned int _width, unsigned int _height)
{
	CreateTexture(_renderer, _width, _height);
}


void FurTexture::GenerateFur(Renderer& _renderer, float _density)
{
	unsigned int texture_size = desc.Height * desc.Width;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_renderer.GetDeviceContext()->Map(m_texture_resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	float* pixel_data = static_cast<float*>(mappedResource.pData);	


	int strand_count = static_cast<int>(_density * texture_size);
	for (unsigned int i = 0; i < strand_count; ++i)
	{
		std::pair<unsigned int, unsigned int> rand_coord = GetRandomTexPosition();// Random coord to place strand.

		// Convert to weird DirectX pixel layout?
		RGBA pixel;
		pixel.r = 255;
		pixel.g = 255;
		pixel.b = 255;
		pixel.a = 255;

		memcpy(&pixel, &pixel_data[desc.Width * rand_coord.first * rand_coord.second], 4 );

	}

	memcpy(mappedResource.pData, pixel_data,  mappedResource.RowPitch);
	_renderer.GetDeviceContext()->Unmap(m_texture_resource, 0);

}


ID3D11ShaderResourceView* FurTexture::GetTexture() const
{
	return m_texture;
}


ID3D11Texture2D * FurTexture::GetTextureResource() const
{
	return m_texture_resource;
}


void FurTexture::CreateTexture(Renderer& _renderer, unsigned int _width, unsigned int _height)
{
	
	desc.Width = _width;
	desc.Height = _height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags =  D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	static const float start_colour = 0;
	D3D11_SUBRESOURCE_DATA start_pixels = { &start_colour, sizeof(float), 0 };//set start pixels all to black with no alpha

	HRESULT result {};
	
	result = _renderer.GetDevice()->CreateTexture2D(&desc, &start_pixels, &m_texture_resource);

	if (result != MB_OK)
	{
		MessageBox(nullptr, "[FurTexture](CreateTexture) Failed to create texture", "Error", MB_OK);
		exit(0);
	}

	result =_renderer.GetDevice()->CreateShaderResourceView(m_texture_resource, nullptr, &m_texture);

	if (result != MB_OK)
	{
		MessageBox(nullptr, "[FurTexture](CreateTexture) Failed to create texture resource view", "Error", MB_OK);
		exit(0);
	}
}


std::pair<unsigned int, unsigned int> FurTexture::GetRandomTexPosition() const
{
	static std::mt19937 m_random;
	std::uniform_int_distribution<int> distribution_x(0, desc.Width);// Ensures different rand value.	
	std::uniform_int_distribution<int> distribution_y(0, desc.Height);

	return std::pair<unsigned int, unsigned int>(distribution_x(m_random), distribution_y(m_random));// Because I'm too lazy to make a vec2.
}
