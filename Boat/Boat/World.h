#pragma once
#include <vector>

#include "Renderer.h"
#include "Entity.h"
#include "PhysicsScene.h"
#include "CameraComponent.h"

class World
{
private:
	Renderer* renderer;
	PhysicsScene* physics_scene;
	std::vector<Entity*> entities;
	CameraComponent* main_camera;

	bool logic_begun = false;
	bool window_begun = false;

	bool is_being_destroyed = false;
	bool logic_destroyed = false;
	bool window_destroyed = false;

public:
	virtual ~World();

	void LogicBegin(class GameLogic* game_logic);
	void WindowBegin(class Window* window);

	void LogicTick(class GameLogic* game_logic, float delta_time);
	void WindowTick(class Window* window, float delta_time);

	inline bool HasLoaded() const { return logic_begun && window_begun; }
	inline void Destroy() { is_being_destroyed = true; }
	inline bool IsDestroyed() { return logic_destroyed && window_destroyed; }

	virtual void LoadLogicResources(class GameLogic* game_logic);
	virtual void LoadWindowResources(class Window* window);
	virtual void UnloadLogicResources(class GameLogic* game_logic);
	virtual void UnloadWindowResources(class Window* window);

	//Memory management control should be passed onto World
	void AddEntity(Entity* entity);

	inline Renderer* GetRenderer() { return renderer; }
	inline PhysicsScene* GetPhysicsScene() { return physics_scene; }

	inline CameraComponent* GetMainCamera() { return main_camera; }
	inline void SetMainCamera(CameraComponent* camera) { main_camera = camera; }
};

