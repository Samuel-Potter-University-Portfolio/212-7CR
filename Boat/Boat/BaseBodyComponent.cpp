#include "BaseBodyComponent.h"
#include "BaseColliderComponent.h"
#include "PhysicsScene.h"

void BaseBodyComponent::JoinScene(PhysicsScene* physics_scene)
{
	this->physics_scene = physics_scene;
}

void BaseBodyComponent::SetAttachedCollider(BaseColliderComponent* collider) 
{ 
	this->collider = collider; 
	if (collider)
		collider->SetAttachedBody(this);
}

void BaseBodyComponent::PhysicsTick(float delta_time) 
{
	//Calculate new velocity for this frame
	const float scale = physics_scene ? physics_scene->GetSettings().metre_scale : 1.0f;
	frame_velocity += current_acceleration * delta_time * scale;
	current_acceleration = glm::vec3(0);

	//Check for collisions
	if (physics_scene)
	{
		const float scale = physics_scene->GetSettings().metre_scale;

		for (BaseColliderComponent* collider : physics_scene->GetColliders())
		{
			if (collider != GetAttachedCollider())
			{
				HitInfo hit;
				collider->HandleCollision(this, frame_velocity, hit);
			}
		}
	}

	//Update position
	parent->transform.location += frame_velocity;

	frame_velocity *= 1.0f - velocity_damping_factor;
}