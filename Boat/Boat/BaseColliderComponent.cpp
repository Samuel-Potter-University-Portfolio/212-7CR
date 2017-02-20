#include "BaseColliderComponent.h"
#include "BaseBodyComponent.h"
#include "glm.hpp"

bool BaseColliderComponent::DoesBroadPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity)
{
	const glm::vec3 a_loc = GetWorldLocation();
	const glm::vec3 b_loc = collider->GetWorldLocation() + velocity;

	const float distance = glm::distance(a_loc, b_loc);
	return distance <= bounding_radius + collider->bounding_radius;
}

void BaseColliderComponent::DefaultResolution(BaseBodyComponent* attached_body, HitInfo& hit_info) 
{
	BaseColliderComponent* collider = attached_body->GetAttachedCollider();
	const float restitution = (properties.restitution + collider->GetProperties().restitution) / 2.0f;
	const float friction = properties.damping_friction_factor;


	const glm::vec3 correction = hit_info.normal * hit_info.distance * (1.0f + restitution);
	attached_body->GetCurrentVelocity() += correction;
	attached_body->GetCurrentVelocity() *= 1.0f - friction;
	attached_body->GetCurrentAngularVelocity() *= 1.0f - friction;

	//Attempt to exert force on this body
	if (properties.restitution != 0.0f)
	{
		BaseBodyComponent* this_body = GetAttachedBody();
		if (this_body)
			this_body->GetCurrentVelocity() -= correction;
	}
}

bool BaseColliderComponent::HandleCollision(BaseBodyComponent* attached_body, glm::vec3& velocity, HitInfo& hit_info)
{
	BaseColliderComponent* collider = attached_body ? attached_body->GetAttachedCollider() : nullptr;

	if (!collider ||
		!DoesBroadPhaseCollide(collider, velocity) ||
		!DoesNarrowPhaseCollide(collider, velocity, hit_info)
		)
		return false;
	
	ResolveCollision(attached_body, hit_info);
	return true;
}
