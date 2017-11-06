#pragma once
#include "Transform.h"


class GameObject
{
public:
	GameObject();
	~GameObject();

protected:
	Transform m_transform;
};