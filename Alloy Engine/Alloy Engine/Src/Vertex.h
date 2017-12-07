#pragma once

class Vector3;


struct Vertex2D
{
	float x, y;
	float r, g, b;
};


struct Vertex3D
{
	Vector3 position;
	float one;//so it maps to float 4
	Vector3 normal;
	float two;
	float u, v;
};