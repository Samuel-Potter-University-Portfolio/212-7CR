#include "RigidBody.h"
#include "PhysicsScene.h"

void RigidBody::PhysicsTick(float delta_time) 
{
	if (physics_scene)
		velocity += physics_scene->GetSettings().gravity * delta_time;

	__super::PhysicsTick(delta_time);
}