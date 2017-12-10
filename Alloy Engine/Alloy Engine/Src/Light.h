#pragma once
#include "Vector3.h"


struct Light
{
	Vector3 position = Vector3::Zero;
	float colour[3] = { 1,1,1 };
	float intensity = 1;

	Light() = default;

	Light(Vector3 _position, float _r, float _g, float _b, float _intensity)
	{
		position = _position;
		colour[0] = _r;
		colour[1] = _g;
		colour[3] = _b;
		intensity = _intensity;
	}
};
