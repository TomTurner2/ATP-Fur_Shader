#include "FurMaterial.h"


void FurMaterial::LoadFurTextures(Renderer& _renderer, std::string _fur_mask_path, std::string _fur_alpha_path)
{
	m_fur_alpha = new Texture(_renderer, _fur_alpha_path);
	m_fur_mask = new Texture(_renderer, _fur_mask_path);
}


void FurMaterial::CreateBuffers(Renderer& _renderer)
{
	Material::CreateBuffers(_renderer);//create base constant buffers
}


//remember to take into account base constant buffer registers when writing shaders
void FurMaterial::UpdateAndAddCustomBuffers()
{
	m_ps_resources.push_back(m_fur_alpha->GetTexture());
	m_ps_resources.push_back(m_fur_mask->GetTexture());

	m_gs_resources.push_back(m_fur_alpha->GetTexture());
	m_gs_resources.push_back(m_fur_mask->GetTexture());
}
