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


Transform GameObject::GetTransform() const
{
	return m_transform;
}
