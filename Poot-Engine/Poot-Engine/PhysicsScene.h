#pragma once
#include "API.h"
#include "Entity.h"
#include "BaseBodyComponent.h"
#include "BaseColliderComponent.h"

#include <vector>
#include <vec3.hpp>

struct PhysicsSettings
{
	glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	float metre_scale = 0.01f;
};


class POOT_API PhysicsScene
{
private:
	std::vector<BaseColliderComponent*> colliders;
	std::vector<BaseBodyComponent*> bodies;
	PhysicsSettings settings;

public:
	PhysicsScene();

	void Link(class World* world);
	void Tick(float delta_time);

	inline PhysicsSettings& GetSettings() { return settings; }
	inline std::vector<BaseColliderComponent*>& GetColliders() { return colliders; }

	void CleanUp();
	void AddEntityToLevel(Entity* entity);
};

