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
	const float scale = physics_scene ? physics_scene->GetSettings().metre_scale : 1.0f;
	frame_velocity += current_acceleration * delta_time * scale;
	current_acceleration = glm::vec3(0);

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

	parent->transform.location += frame_velocity;
}