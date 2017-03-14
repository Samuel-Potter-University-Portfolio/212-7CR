#include "PlaneCollider.h"
#include "SphereCollider.h"
#include <glm.hpp>


void PlaneCollider::ResolveCollision(Body* body, HitInfo& hit_info)
{

	Collider* collider = body->GetCollider();

	//Resolve Sphere
	{
		SphereCollider* sphere = Cast<SphereCollider>(collider);

		if (sphere)
		{
			DefaultResolution(body, hit_info);
			return;
		}
	}
}

float DistanceToPlane(PlaneCollider& plane, SphereCollider* sphere, glm::vec3 velocity)
{
	const float length = plane.normal.length();

	if (length == 0.0f)
		plane.normal = glm::vec3(0, 1, 0);

	if (length != 1.0f)
		glm::normalize(plane.normal);

	//ax + by + cz + d = 0
	const float d = -(glm::dot(plane.normal, plane.GetWorldLocation()));
	return glm::dot(plane.normal, sphere->GetWorldLocation() + velocity) + d - sphere->radius;
}

bool PlaneCollider::DoesBroadPhaseCollide(Collider* collider, glm::vec3 velocity)
{
	//Assume always hit
	return true;
}

bool PlaneCollider::DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info) 
{
	//Sphere on Sphere
	{
		SphereCollider* sphere = Cast<SphereCollider>(collider);

		if (sphere)
		{
			const float distance = DistanceToPlane(*this, sphere, velocity);
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