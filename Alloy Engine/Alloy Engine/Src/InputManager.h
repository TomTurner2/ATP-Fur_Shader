#pragma once
#include <Windows.h>
#include "KeyState.h"
#include "GameAction.h"
#include <map>

class InputManager
{
public:
	InputManager() = default;
	~InputManager() = default;

	enum TriggerType : unsigned int// States that can be checked.
	{
		PRESSED,
		HELD
	};

	void ProcessMessage(UINT _msg, WPARAM _wParam);
	bool GetGameAction(GameAction _action, TriggerType _state = PRESSED);
	void BindKey(GameAction _action, std::string _key);

private:
	std::map<GameAction, KeyState> m_key_binds;
};

