#include "SphereCollider.h"
#include "BaseBodyComponent.h"
#include <glm.hpp>


SphereCollider::SphereCollider()
{
	bounding_radius = 1.0f;
}

bool SphereCollider::DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info)
{
	if ((SphereCollider*)collider && DoesBroadPhaseCollide(collider, velocity))
	{
		hit_info.normal = glm::normalize(GetWorldLocation() - collider->GetWorldLocation());
		hit_info.location = collider->GetWorldLocation() + collider->GetBoundingRadius() * hit_info.normal;
		return true;
	}

	return false;
}

void SphereCollider::ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info)
{
}