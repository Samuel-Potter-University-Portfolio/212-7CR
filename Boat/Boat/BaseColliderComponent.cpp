#include "BaseColliderComponent.h"
#include "BaseBodyComponent.h"
#include "glm.hpp"

bool BaseColliderComponent::DoesBroadPhaseCollide(BaseColliderComponent* collider) 
{
	const glm::vec3 a_loc = GetWorldLocation();
	const glm::vec3 b_loc = collider->GetWorldLocation();

	const float distance = glm::distance(a_loc, b_loc);
	return distance <= bounding_radius + collider->bounding_radius;
}

bool BaseColliderComponent::ResolveCollisions(BaseBodyComponent* attached_body, HitInfo& hit_info)
{
	BaseColliderComponent* collider = attached_body ? attached_body->GetAttachedCollider() : nullptr;

	if (!collider || 
		!DoesBroadPhaseCollide(collider) || !collider->DoesBroadPhaseCollide(this) ||
		!DoesNarrowPhaseCollide(collider, hit_info) || !collider->DoesNarrowPhaseCollide(this, hit_info)
	)
		return false;
	
	return true;
}