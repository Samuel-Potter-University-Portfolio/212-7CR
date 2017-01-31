#pragma once


class World
{
private:
	bool logic_begun = false;
	bool window_begun = false;

	bool is_being_destroyed = false;
	bool logic_destroyed = false;
	bool window_destroyed = false;

public:
	void LogicBegin(class GameLogic* game_logic);
	void WindowBegin(class Window* window);

	void LogicTick(class GameLogic* game_logic, float delta_time);
	void WindowTick(class Window* window, float delta_time);

	inline bool HasLoaded() const { return logic_begun && window_begun; }
	inline void Destroy() { is_being_destroyed = true; }
	inline bool IsDestroyed() { return logic_destroyed && window_destroyed; }

	void LoadLogicResources(class GameLogic* game_logic);
	void LoadWindowResources(class Window* window);
	void UnloadLogicResources(class GameLogic* game_logic);
	void UnloadWindowResources(class Window* window);
};

