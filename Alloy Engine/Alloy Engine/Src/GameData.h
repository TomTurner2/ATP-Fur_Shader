#pragma once
#include "InputManager.h"


struct GameData
{
	float delta_time;
	float fixed_delta_time;// Used for physics update.
	InputManager* input;
};
