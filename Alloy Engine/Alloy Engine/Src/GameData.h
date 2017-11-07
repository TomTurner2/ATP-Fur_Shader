#pragma once
#include "InputManager.h"
#include <memory>


struct GameData
{
	float delta_time;
	float fixed_delta_time;//used for physics update
	InputManager* input;
};
