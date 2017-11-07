#pragma once
#include <string>


struct KeyState
{
	int key;
	bool down;
	bool oldDown;

	KeyState(int _key)
	{
		key = _key;
		down = false;
		oldDown = false;
	}
};
