#pragma once
#include "Material.h"

class Texture;

class FurMaterial : public Material
{
public:
	void LoadFurTextures(Renderer& _renderer, std::string _fur_mask_path, std::string _fur_alpha_path);

protected:
	void CreateBuffers(Renderer& _renderer) override;
	void UpdateAndAddCustomBuffers() override;

	Texture* m_fur_alpha{ nullptr };
	Texture* m_fur_mask{ nullptr };
};
