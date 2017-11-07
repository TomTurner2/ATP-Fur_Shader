#include "InputManager.h"


void InputManager::ProcessMessage(UINT _msg, WPARAM _wParam)
{
	switch (_msg)
	{
	case WM_KEYDOWN:
		for (auto& keyBind : key_binds)
		{
			keyBind.second.oldDown = keyBind.second.down;//set previous state

			if (_wParam == keyBind.second.key)
				keyBind.second.down = true;//set new state
		}
		break;
	case WM_KEYUP:
		for (auto& keyBind : key_binds)
		{
			keyBind.second.oldDown = keyBind.second.down;//set previous state

			if (_wParam == keyBind.second.key)
				keyBind.second.down = false;//set new state
		}
		break;
	}	
}


bool InputManager::GetGameAction(GameAction _action, TriggerType _state)
{
	auto find_key = key_binds.find(_action);

	if (find_key != key_binds.end() && (*find_key).second.down)
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

	if (key_binds.find(_action) == key_binds.end())
	{
		key_binds.insert(std::make_pair(_action, KeyState(ascii)));
	}
	else
	{
		key_binds.at(_action) = KeyState(ascii);
	}
}