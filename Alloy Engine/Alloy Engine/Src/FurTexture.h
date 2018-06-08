#pragma once
#include "Texture.h"


class Renderer;

// This was abandoned as DirectX texture creation is disgusting.
class FurTexture
{
public:
	FurTexture(Renderer& _renderer, unsigned int _width, unsigned int _height);
	~FurTexture() = default;

	void GenerateFur(Renderer& _renderer, float _density);

	ID3D11ShaderResourceView* GetTexture() const;
	ID3D11Texture2D* GetTextureResource() const;
	
private:
	ID3D11Texture2D* m_texture_resource { nullptr };
	ID3D11ShaderResourceView* m_texture { nullptr };
	D3D11_TEXTURE2D_DESC desc;

	struct RGBA
	{
		float a;
		float b;
		float g;
		float r;
	};
	
	void CreateTexture(Renderer& _renderer, unsigned int _width, unsigned int _height);
	std::pair<unsigned int, unsigned int> GetRandomTexPosition() const;
};
