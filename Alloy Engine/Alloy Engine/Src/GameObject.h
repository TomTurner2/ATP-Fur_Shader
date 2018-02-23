#pragma once
#include "Transform.h"
#include "GameData.h"


class Renderer;


class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual void Tick(GameData& _game_data);
	virtual void Draw(Renderer& _renderer);

	void SetTransform(Transform _transform);
	void SetActive(bool _active);
	void SetCanDraw(bool _can_draw);
	void SetCanTick(bool _can_tick);

	bool GetActive() const;
	bool GetCanDraw() const;
	bool GetCanTick() const;

	Transform GetTransform() const;

protected:
	Transform m_transform;
	bool m_can_draw = true;
	bool m_can_tick = true;
};