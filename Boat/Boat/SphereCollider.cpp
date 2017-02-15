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
			const float restitution = (properties.restitution + sphere->GetProperties().restitution) / 2.0f;
			const float friction = properties.damping_friction_factor;

			const glm::vec3 correction = hit_info.normal *(hit_info.distance) * (1.0f + restitution);
			attached_body->GetCurrentVelocity() += correction;
			attached_body->GetCurrentVelocity() *= 1.0f - friction;
			attached_body->GetCurrentAngularVelocity() *= 1.0f - friction;

			//Attempt to exert force on this body
			if (properties.restitution != 0.0f)
			{
				BaseBodyComponent* this_body = GetAttachedBody();
				if(this_body)
					this_body->GetCurrentVelocity() -= correction;
			}
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
