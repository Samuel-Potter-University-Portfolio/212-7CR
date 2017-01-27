#pragma once

#include <string>
#include <vec2.hpp>

#include "Window.h"


extern class Game* g_game;


class Game
{
private:
	std::string game_name;
	Window* window;

public:
	Game(const std::string game_name, const glm::ivec2 desired_resolution);
	~Game();

	void Launch();

	inline std::string GetName() { return game_name; }
};

