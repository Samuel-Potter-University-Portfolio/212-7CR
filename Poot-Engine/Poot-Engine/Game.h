#pragma once
#include "API.h"

#include <string>
#include <vec2.hpp>

#include "Window.h"
#include "GameLogic.h"
#include "World.h"

extern POOT_API class Game* g_game;


class POOT_API Game
{
private:
	std::string game_name;
	Window* window;
	GameLogic* game_logic;
	World* world;

	bool logic_ready;
	bool window_ready;
	bool closed_requested;

public:
	Game(const std::string game_name, const glm::ivec2 desired_resolution, World* world);
	~Game();

	void Launch();
	inline void Close() { closed_requested = true; }

	void SetLogicReady();
	void SetWindowReady();

	inline bool IsReady() { return window_ready && logic_ready; }
	inline bool IsClosedRequested() { return closed_requested; }
	inline std::string GetName() { return game_name; }

	inline World* GetWorld() { return world; }
	inline Window* GetWindow() { return window; }
	inline GameLogic* GetGameLogic() { return game_logic; }

private:
	void CleanUp();
};

