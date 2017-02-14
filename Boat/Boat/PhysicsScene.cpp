#include "PhysicsScene.h"
#include "Logger.h"
#include "World.h"
#include "Game.h"

PhysicsScene::PhysicsScene()
{
}

void PhysicsScene::Tick(float delta_time) 
{
	for (BaseBodyComponent* body : bodies)
	{
		if (body->IsActive() && body->IsAwake())
			body->PhysicsTick(delta_time);
	}
}

void PhysicsScene::Link(World* world) 
{
}

void PhysicsScene::CleanUp() 
{
}

void PhysicsScene::AddEntityToLevel(Entity* entity)
{
	if (!entity)
		return;

	auto entity_bodies = entity->GetAllComponents<BaseBodyComponent>();

	//Bodies are only allow one collider attached
	if (entity_bodies.size() != 0)
	{
		if (entity_bodies.size() > 1)
			LOG(Error, "An entity may only contains 1 body component");

		BaseBodyComponent* body = entity_bodies[0];
		bodies.push_back(body);
		body->JoinScene(this);


		//Check for 1 collider
		auto entity_colliders = entity->GetAllComponents<BaseColliderComponent>();

		if (entity_colliders.size() != 0)
		{
			if (entity_colliders.size() > 1)
				LOG(Error, "An entity may only contains 1 collider, if it has a body");

			BaseColliderComponent* collider = entity_colliders[0];
			colliders.push_back(collider);
			body->SetAttachedCollider(collider);
		}
	}

	//No body, so add all colliders
	else
	{
		for (BaseColliderComponent* collider : entity->GetAllComponents<BaseColliderComponent>())
			colliders.push_back(collider);
	}
}