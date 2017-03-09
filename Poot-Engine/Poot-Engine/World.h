#pragma once
#include "API.h"
#include <vector>
#include <queue>

#include "MasterRenderer.h"
#include "GameObject.h"
//TODO - UPDATE #include "PhysicsScene.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"

class POOT_API World
{
private:
	MasterRenderer master_renderer;
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


	inline MasterRenderer& GetMasterRenderer() { return master_renderer; }
	//TODO - UPDATE inline PhysicsScene* GetPhysicsScene() { return physics_scene; }

	inline CameraComponent* GetMainCamera() { return main_camera; }
	inline void SetMainCamera(CameraComponent* camera) { main_camera = camera; }

	inline DirectionalLightComponent* GetSunLight() { return sun_light; }
	inline void SetSunLight(DirectionalLightComponent* sun) { sun_light = sun; }


//Object control
public:
	//Memory management control should be passed onto World
	void AddObject(GameObject* object);

private:
	void InternalSpawnObject(GameObject* object);
	void InternalAddComponent(Component* component);


//Object fetching
public:
	inline const std::vector<GameObject*>& GetAllObjects() { return game_objects; };

	inline const std::vector<GameObject*>& GetAllObjects(Tag tags = OBJ_TAG_ALL) 
	{
		std::vector<GameObject*> objects;

		for (GameObject* object : game_objects)
		{
			if (object->HasTag(tags))
				objects.push_back(object);
		}
		return objects;
	};

	template<typename Obj>
	inline Obj* GetFirstObject(Tag tags = OBJ_TAG_ALL)
	{
		for (GameObject* object : game_objects)
		{
			Obj* actual_obj = Cast<Obj>(object);
			if (actual_obj && object->HasTag(tags))
				return actual_obj;
		}
		return nullptr;
	}
	template<typename Obj>
	inline std::vector<Obj*> GetAllObjects(Tag tags = OBJ_TAG_ALL)
	{
		std::vector<Obj*> objects;

		for (GameObject* object : game_objects)
		{
			Obj* actual_obj = Cast<Obj>(object);
			if (actual_obj && object->HasTag(tags))
				objects.push_back(actual_obj);
		}
		return objects;
	}
};

