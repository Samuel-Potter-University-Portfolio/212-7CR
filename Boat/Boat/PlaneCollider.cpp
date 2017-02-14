#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BaseBodyComponent.h"
#include <glm.hpp>

void PlaneCollider::ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info) 
{
	BaseColliderComponent* collider = attached_body->GetAttachedCollider();

	//Resolve sphere
	{
		SphereCollider* sphere = (SphereCollider*)collider;
		if (sphere)
		{
			const float restitution = (properties.restitution + sphere->GetProperties().restitution) / 2.0f;
			const float friction = properties.damping_friction_factor;

			attached_body->GetCurrentVelocity() -= (hit_info.normal * (hit_info.distance)) * (1.0f + restitution);
			attached_body->GetCurrentVelocity() *= 1.0f - friction;
		}
	}
}

float PlaneCollider::DistanceToPlane(SphereCollider* sphere, glm::vec3 velocity)
{
	const float length = normal.length();

	if (length == 0.0f)
		normal = glm::vec3(0, 1, 0);

	if (length != 1.0f)
		glm::normalize(normal);

	//ax + by + cz + d = 0
	const float d = -(glm::dot(normal, GetWorldLocation()));
	return glm::dot(normal, sphere->GetWorldLocation() + velocity) + d - sphere->GetRadius();
}

bool PlaneCollider::DoesBroadPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity)
{
	return true;
}

bool PlaneCollider::DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info)
{
	//Check sphere
	{
		SphereCollider* sphere = (SphereCollider*)collider;
		if (sphere)
		{
			const float distance = DistanceToPlane(sphere, velocity);
			if (distance <= 0.0f)
			{
				hit_info.normal = normal;
				hit_info.distance = distance;
				return distance <= 0;
			}
		}
	}

	return false;
}