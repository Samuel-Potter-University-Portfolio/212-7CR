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
