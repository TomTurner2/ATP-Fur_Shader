#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	Vector3 m_pos = Vector3::Zero;
};