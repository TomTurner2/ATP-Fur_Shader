#pragma once
#include "Vector3.h"

struct FurParameters
{
	float fur_mask_multiplier = 1;
	float max_fur_length = 2;
	int layer_count = 5;
	float base_clip = 0.5f;
	float end_clip = 0.1f;
	Vector3 gravity = Vector3(0, 0, 0);
};
