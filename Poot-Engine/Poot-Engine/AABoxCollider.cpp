#include "AABoxCollider.h"
#include "BaseBodyComponent.h"
#include "SphereCollider.h"
#include "common.hpp"
#include "glm.hpp"

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)


AABoxCollider::AABoxCollider()
{
	bounding_radius = transform.scale.length() * transform.scale.length();
}
   
void AABoxCollider::ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info)
{
	BaseColliderComponent* collider = attached_body->GetAttachedCollider();

	//Resolve sphere
	{
		SphereCollider* sphere = (SphereCollider*)collider;
		if (sphere)
		{
			DefaultResolution(attached_body, hit_info);
			return;
		}
	}
}

inline float SideDistanceSqrd(float point, float min, float max)
{
	if (point < min)
		return (min - point)*(min - point);

	else if (point > max)
		return (point - max)*(point - max);

	return 0.0f;
}

inline float SideDepth(float point, float radius, float min, float max)
{
	const float max_dist = (point - radius) - max;
	const float min_dist = min - (point + radius);

	if (min > 0.0f || max < 0.0f)
		return min_dist > max_dist ? min_dist : -max_dist;
	else
		return 0.0f;
}

bool AABoxCollider::DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info) 
{
	//Test sphere
	{
		//Source https://studiofreya.com/3d-math-and-physics/sphere-vs-aabb-collision-detection-test/
		//https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

		if ((SphereCollider*)collider)
		{
			SphereCollider* sphere = (SphereCollider*)collider;
			const glm::vec3 this_location = GetWorldLocation();
			const glm::vec3 this_half_scale = GetWorldScale() / 2.0f;

			const glm::vec3 min_corner = this_location - this_half_scale;
			const glm::vec3 max_corner = this_location + this_half_scale;
			const glm::vec3 sphere_location = sphere->GetWorldLocation() + velocity;

			const glm::vec3 closet_point = glm::vec3(
				MAX(min_corner.x, MIN(sphere_location.x, max_corner.x)),
				MAX(min_corner.y, MIN(sphere_location.y, max_corner.y)),
				MAX(min_corner.z, MIN(sphere_location.z, max_corner.z))
				);

			const float distance = glm::distance(closet_point, sphere_location);

			if (distance <= sphere->GetRadius())
			{
				hit_info.distance = sphere->GetRadius() - distance;
				const glm::vec3 normal = sphere_location - closet_point;
				hit_info.normal = normal.length() != 0 ? glm::normalize(normal) : normal;
				return true;
			}
			else
				return false;
		}	
	}
	
	return false;
}
