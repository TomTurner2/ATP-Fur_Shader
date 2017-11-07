#pragma once
#include "Transform.h"
#include "GameData.h"


class Renderer;


class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;

	virtual void Tick(GameData& _game_data);
	virtual void Draw(Renderer& _renderer);

	Transform GetTransform() const;

protected:
	Transform m_transform;
};