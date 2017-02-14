#include "AABoxCollider.h"
#include "SphereCollider.h"
#include "common.hpp"
#include "glm.hpp"


AABoxCollider::AABoxCollider()
{
	bounding_radius = transform.scale.length();
}

void AABoxCollider::ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info)
{

}

bool AABoxCollider::DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info) 
{
	//Test sphere
	{
		if ((SphereCollider*)collider)
		{
			hit_info.normal = glm::normalize(GetWorldLocation() - collider->GetWorldLocation());
			hit_info.location = collider->GetWorldLocation() + collider->GetBoundingRadius() * hit_info.normal;
			return true;
		}
	}

	return false;
}
