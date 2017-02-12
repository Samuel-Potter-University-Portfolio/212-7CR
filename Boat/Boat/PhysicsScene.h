#pragma once
#include "Entity.h"
#include "BaseBodyComponent.h"
#include "BaseColliderComponent.h"
#include <vector>

struct PhysicsSettings
{
	float gravity = 9.8f;
	float metre_scale = 1.0f;
};


class PhysicsScene
{
private:
	std::vector<BaseColliderComponent*> colliders;
	std::vector<BaseBodyComponent*> bodies;
	PhysicsSettings settings;

public:
	PhysicsScene();

	void Tick(float delta_time);

	inline PhysicsSettings& GetSettings() { return settings; }

	void CleanUp();
	void AddEntityToLevel(Entity* entity);
};

