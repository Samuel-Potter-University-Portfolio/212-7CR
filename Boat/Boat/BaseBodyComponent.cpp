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
	float scale = 1.0f;

	if (physics_scene)
		scale = physics_scene->GetSettings().metre_scale;

	parent->transform.location += velocity * delta_time * scale;
}