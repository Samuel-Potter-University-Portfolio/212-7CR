#include "SphereCollider.h"
#include <glm.hpp>



SphereCollider::SphereCollider()
	: radius(bounding_radius)
{
}

void SphereCollider::ResolveCollision(Body* body, HitInfo& hit_info) 
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

bool SphereCollider::DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info) 
{
	//Sphere on Sphere
	{
		SphereCollider* sphere = Cast<SphereCollider>(collider);

		if (sphere)
		{
			//Broad phase uses bounding spheres, as must have already collided
			hit_info.normal = glm::normalize((sphere->GetWorldLocation() + velocity) - GetWorldLocation());
			hit_info.embedded_distance = glm::distance((sphere->GetWorldLocation() + velocity), GetWorldLocation()) - radius - sphere->radius;
			return true;
		}
	}

	return false;
}
