#include "DebugSystem.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "Renderer.h"
#include "AntTweakBar.h"


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


void DebugSystem::InitAntTweakBar(Renderer& _renderer) const
{
	TwInit(TW_DIRECT3D11, _renderer.GetDevice());
	TwWindowSize(_renderer.GetViewportDimensions().first, _renderer.GetViewportDimensions().second);
}


void DebugSystem::DrawDebugUI() const
{
	TwDraw();
}


void DebugSystem::Log(std::string _message)
{
	if (AllocConsole())//only allow print if a console is allocated
		return;
	
	printf("%s\n", _message.c_str());
}


void DebugSystem::LogWarning(std::string _message)
{
	if (AllocConsole())
		return;

	printf("[Warning] %s\n", _message.c_str());
}


void DebugSystem::LogError(std::string _message)
{
	if (AllocConsole())
		return;

	printf("[Error] %s\n", _message.c_str());
}
