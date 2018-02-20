#pragma once
#include <string>


class Renderer;

class DebugSystem
{
public:
	DebugSystem() = default;
	~DebugSystem() = default;

	void CreateConsoleWindow() const;
	void InitAntTweakBar(Renderer& _renderer) const;
	void DrawDebugUI() const;

	static void Log(std::string _message);
	static void LogWarning(std::string _message);
	static void LogError(std::string _message);
};