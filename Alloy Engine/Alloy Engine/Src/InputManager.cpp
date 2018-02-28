#include "InputManager.h"


void InputManager::ProcessMessage(UINT _msg, WPARAM _wParam)
{
	switch (_msg)
	{
	case WM_KEYDOWN:
		for (auto& keyBind : m_key_binds)
		{
			keyBind.second.oldDown = keyBind.second.down;// Set previous state.

			if (_wParam == keyBind.second.key)
				keyBind.second.down = true;// Set new state.
		}
		break;
	case WM_KEYUP:
		for (auto& keyBind : m_key_binds)
		{
			keyBind.second.oldDown = keyBind.second.down;// Set previous state.

			if (_wParam == keyBind.second.key)
				keyBind.second.down = false;// Set new state.
		}
		break;
	}	
}


bool InputManager::GetGameAction(GameAction _action, TriggerType _state)
{
	auto find_key = m_key_binds.find(_action);

	if (find_key != m_key_binds.end() && (*find_key).second.down)
	{
		switch (_state)
		{
		case PRESSED: return ((*find_key).second.down && !(*find_key).second.oldDown);
		case HELD: return ((*find_key).second.down && (*find_key).second.oldDown);
		default:;
		}
	}

	return false;
}


void InputManager::BindKey(GameAction _action, std::string _key)
{
	int ascii = _key.at(0);

	if (m_key_binds.find(_action) == m_key_binds.end())
	{
		m_key_binds.insert(std::make_pair(_action, KeyState(ascii)));
	}
	else
	{
		m_key_binds.at(_action) = KeyState(ascii);
	}
}