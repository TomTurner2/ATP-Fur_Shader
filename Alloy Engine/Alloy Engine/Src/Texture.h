#pragma once
#include <string>
#include <d3d11.h>


class Renderer;


class Texture
{
public:
	Texture() = default;
	Texture(Renderer& _renderer, std::string _path);
	~Texture() = default;

	void LoadTexture(Renderer& _renderer, std::string _path);
	ID3D11ShaderResourceView* GetTexture() const;
	ID3D11Resource* GetTextureResource() const;

protected:
	ID3D11ShaderResourceView* m_texture { nullptr };
	ID3D11Resource* m_texture_resource { nullptr };
};