#include "GameObject.h"


GameObject::GameObject()
{
}


void GameObject::Tick(GameData & _game_data)
{
}


void GameObject::Draw(Renderer & _renderer)
{
}


void GameObject::SetTransform(Transform _transform)
{
	m_transform = _transform;
}


void GameObject::SetActive(bool _active)
{
	m_can_draw = _active;
	m_can_tick = _active;
}


void GameObject::SetCanDraw(bool _can_draw)
{
	m_can_draw = _can_draw;
}


void GameObject::SetCanTick(bool _can_tick)
{
	m_can_tick = _can_tick;
}


bool GameObject::GetActive() const
{
	return (m_can_tick && m_can_draw);
}


bool GameObject::GetCanDraw() const
{
	return m_can_draw;
}


bool GameObject::GetCanTick() const
{
	return m_can_tick;
}


Transform GameObject::GetTransform() const
{
	return m_transform;
}
