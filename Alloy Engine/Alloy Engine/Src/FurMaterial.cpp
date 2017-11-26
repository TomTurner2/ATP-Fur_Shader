#include "FurMaterial.h"


void FurMaterial::CreateBuffers(Renderer& _renderer)
{
	Material::CreateBuffers(_renderer);

	//create any additional buffers
}

//remember to take into account base buffer registers when writing shaders
void FurMaterial::UpdateAndAddCustomBuffers()
{
	//map then add any non base buffers
}
