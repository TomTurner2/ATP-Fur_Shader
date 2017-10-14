#include "DebugSystem.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>


void DebugSystem::CreateConsoleWindow() const
{
	if (AllocConsole()) 
	{
		freopen("CONOUT$", "w", stdout);
		SetConsoleTitle("Alloy Engine - Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		AttachConsole(GetCurrentProcessId());
		std::cout << "Debug Console Initialised."<<std::endl;
	}
}
