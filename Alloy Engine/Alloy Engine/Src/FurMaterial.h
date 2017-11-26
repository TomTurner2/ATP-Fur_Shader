#pragma once
#include "Material.h"

class FurMaterial : public Material
{
public:
	void UpdateBuffers(Renderer& _renderer) override;
protected:
	void CreateBuffers(Renderer& _renderer) override;
};
