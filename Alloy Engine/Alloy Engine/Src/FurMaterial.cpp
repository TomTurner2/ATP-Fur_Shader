#include "FurMaterial.h"


void FurMaterial::CreateBuffers(Renderer& _renderer)
{
	Material::CreateBuffers(_renderer);//create base constant buffers

	//create any additional constant buffers
}


//remember to take into account base constant buffer registers when writing shaders
void FurMaterial::UpdateAndAddCustomBuffers()
{
	//map then add any material specific constant buffers
}
