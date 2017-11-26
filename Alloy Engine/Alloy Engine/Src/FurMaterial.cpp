#include "FurMaterial.h"

void FurMaterial::UpdateBuffers(Renderer& _renderer)
{
	Material::UpdateBuffers(_renderer);

	//Update any additional buffers
}


void FurMaterial::CreateBuffers(Renderer& _renderer)
{
	Material::CreateBuffers(_renderer);

	//create any additional buffers
}
