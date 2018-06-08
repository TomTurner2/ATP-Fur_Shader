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
	float one;// Padding.
	Vector3 normal;
	float two;
	float u, v;
};