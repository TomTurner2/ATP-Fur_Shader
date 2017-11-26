#pragma once
#include "Matrix.h"
#include "Vector3.h"
#include "Light.h"
#include "PBRMaterialParams.h"

struct RenderData
{
	Matrix camera_view_matrix;
	Matrix camera_projection_matrix;
	Vector3 camera_pos;
	Light light;
};
