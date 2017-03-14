#pragma once
#include "API.h"
#include "Component.h"
#include "Collider.h"
#include "Body.h"

#include <string>
#include <vec3.hpp>
#include <vector>


struct WorldSettings 
{
	glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	float unit_scale = 0.07f;
	int checks_per_tick = 3;
};


class POOT_API PhysicsScene
{
private:
	class World* world = nullptr;
	WorldSettings settings;

	std::vector<Collider*> colliders;
	std::vector<Body*> bodies;

	std::vector<Collider*> check_buffer;

public:
	void Register(World* world);
	void HandleNewComponent(Component* component);

	inline WorldSettings& GetWorldSettings() { return settings; }
	std::string GetStatusString();

	void PhysicsTick(float delta_time);
	void CleanUp();
};

