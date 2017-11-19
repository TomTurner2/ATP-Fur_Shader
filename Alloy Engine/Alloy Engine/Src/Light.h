#pragma once
#include "Vector3.h"


struct Light
{
	Vector3 position = Vector3::Zero;
	float r = 1, g = 1, b = 1;

	Light() = default;

	Light(Vector3 _position, float _r, float _g, float _b)
	{
		position = _position;
		r = _r;
		g = _g;
		b = _b;
	}
};
