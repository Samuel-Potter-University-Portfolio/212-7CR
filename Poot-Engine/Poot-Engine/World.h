#pragma once
#include "API.h"
#include <vector>
#include <queue>

#include "Renderer.h"
#include "Entity.h"
#include "PhysicsScene.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"

class POOT_API World
{
private:
	Renderer* renderer;
	PhysicsScene* physics_scene;

	std::vector<Entity*> entities;
	std::queue<Entity*> new_entities;
	
	void InternalAddEntity(Entity* entity);

	CameraComponent* main_camera;
	DirectionalLightComponent* sun_light;

	bool logic_begun = false;
	bool window_begun = false;

	bool in_logic_tick = false;

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


	inline Renderer* GetRenderer() { return renderer; }
	inline PhysicsScene* GetPhysicsScene() { return physics_scene; }

	inline CameraComponent* GetMainCamera() { return main_camera; }
	inline void SetMainCamera(CameraComponent* camera) { main_camera = camera; }

	inline DirectionalLightComponent* GetSunLight() { return sun_light; }
	inline void SetSunLight(DirectionalLightComponent* sun) { sun_light = sun; }


//Entity control
public:
	//Memory management control should be passed onto World
	void AddEntity(Entity* entity);

	//Component fetching
	template<typename EntType>
	EntType* GetFirstEntity()
	{
		for (Entity* ent : entities)
		{
			Entity* actual_ent = dynamic_cast<Entity*>(ent);
			if (actual_ent)
				return actual_ent;
		}
		return nullptr;
	}

	template<typename EntType>
	std::vector<EntType*> GetAllEntities()
	{
		std::vector<EntType*> actual_ents;

		for (Entity* ent : entities)
		{
			EntType* actual_ent = dynamic_cast<EntType*>(ent);
			if (actual_ent)
				actual_ents.push_back(actual_ent);
		}
		return actual_ents;
	}
};

