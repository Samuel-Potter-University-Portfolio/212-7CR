#include "RigidBody.h"
#include "PhysicsScene.h"

void RigidBody::PhysicsTick(float delta_time) 
{
	if (physics_scene)
		AddAcceleration(physics_scene->GetSettings().gravity);

	__super::PhysicsTick(delta_time);
}