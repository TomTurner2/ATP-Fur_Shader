#pragma once
#include "Material.h"

class FurMaterial : public Material
{
protected:
	void CreateBuffers(Renderer& _renderer) override;
	void UpdateAndAddCustomBuffers() override;
};
