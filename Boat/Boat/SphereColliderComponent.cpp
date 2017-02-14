#include "SphereColliderComponent.h"
#include "BaseBodyComponent.h"
#include <glm.hpp>


SphereColliderComponent::SphereColliderComponent()
{
	bounding_radius = 1.0f;
}

bool SphereColliderComponent::DoesNarrowPhaseCollide(BaseColliderComponent* collider, HitInfo& hit_info)
{
	if ((SphereColliderComponent*)collider && DoesBroadPhaseCollide(collider))
	{
		hit_info.normal = glm::normalize(GetWorldLocation() - collider->GetWorldLocation());
		hit_info.location = collider->GetWorldLocation() + collider->GetBoundingRadius() * hit_info.normal;
		return true;
	}

	return false;
}

bool SphereColliderComponent::ResolveCollisions(BaseBodyComponent* attached_body, HitInfo& hit_info) 
{
	if (!__super::ResolveCollisions(attached_body, hit_info))
		return false;

	BaseColliderComponent* collider = attached_body->GetAttachedCollider();
}
