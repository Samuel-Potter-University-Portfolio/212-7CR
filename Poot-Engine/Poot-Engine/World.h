#pragma once
#include "API.h"
#include <vector>
#include <queue>

#include "Renderer.h"
#include "GameObject.h"
//TODO - UPDATE #include "PhysicsScene.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"

class POOT_API World
{
private:
	Renderer* renderer;
	//TODO - UPDATE PhysicsScene* physics_scene;

	std::vector<GameObject*> game_objects;
	std::queue<GameObject*> new_objects;
	
	CameraComponent* main_camera;
	DirectionalLightComponent* sun_light;

	bool logic_begun;
	bool window_begun;

	bool in_logic_tick;

	bool is_being_destroyed;
	bool logic_destroyed;
	bool window_destroyed;

public:
	World();
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
	//TODO - UPDATE inline PhysicsScene* GetPhysicsScene() { return physics_scene; }

	inline CameraComponent* GetMainCamera() { return main_camera; }
	inline void SetMainCamera(CameraComponent* camera) { main_camera = camera; }

	inline DirectionalLightComponent* GetSunLight() { return sun_light; }
	inline void SetSunLight(DirectionalLightComponent* sun) { sun_light = sun; }


//Entity control
public:
	//Memory management control should be passed onto World
	void AddObject(GameObject* object);

private:
	void InternalSpawnObject(GameObject* object);
	void InternalAddComponent(Component* component);

	/*
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
	*/
};

