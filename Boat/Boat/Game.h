#pragma once

#include <string>
#include <vec2.hpp>

#include "Window.h"
#include "GameLogic.h"


extern class Game* g_game;


class Game
{
private:
	Window* window;
	GameLogic* game_logic;
	std::string game_name;

	bool logic_ready = false;
	bool window_ready = false;
	bool closed_requested = false;

public:
	Game(const std::string game_name, const glm::ivec2 desired_resolution);
	~Game();

	void Launch();
	inline void Close() { closed_requested = true; }

	void SetLogicReady();
	void SetWindowReady();

	inline bool IsReady() { return window_ready && logic_ready; }
	inline bool IsClosedRequested() { return closed_requested; }
	inline std::string GetName() { return game_name; }

private:
	void CleanUp();
};

