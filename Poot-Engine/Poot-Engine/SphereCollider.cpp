#include "SphereCollider.h"
#include "BaseBodyComponent.h"
#include <glm.hpp>


SphereCollider::SphereCollider()
{
	bounding_radius = 1.0f;
}

void SphereCollider::ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info)
{
	BaseColliderComponent* collider = attached_body->GetAttachedCollider();

	//Resolve sphere
	{
		SphereCollider* sphere = (SphereCollider*)collider;
		if (sphere)
		{
			if (hit_info.distance > -0.0001f)
				hit_info.distance = -0.0001f;

			DefaultResolution(attached_body, hit_info);
			return;
		}
	}
}

bool SphereCollider::DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info)
{
	if ((SphereCollider*)collider && DoesBroadPhaseCollide(collider, velocity))
	{
		SphereCollider* sphere = (SphereCollider*)collider;

		hit_info.normal = glm::normalize(GetWorldLocation() - (collider->GetWorldLocation() + velocity));
		hit_info.distance = glm::distance(GetWorldLocation(), (collider->GetWorldLocation() + velocity)) - sphere->GetRadius() - GetRadius();
		return true;
	}

	return false;
}
