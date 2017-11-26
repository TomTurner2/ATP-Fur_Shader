#pragma once

struct PBRMaterialParams
{
	float roughness = 0.1f;
	float diffuse[3] = {.5f,.5f,.5f};
	float specular[3] = { 0.8f, 0.8f, 0.8f };
};