#pragma once


class Renderer;

class DebugSystem
{
public:
	DebugSystem() = default;
	~DebugSystem() = default;

	void CreateConsoleWindow() const;
	void InitAntTweakBar(Renderer& _renderer) const;
	void DrawDebugUI() const;
};