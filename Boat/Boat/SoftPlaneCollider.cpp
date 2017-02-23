#include "SoftPlaneCollider.h"
#include "BaseBodyComponent.h"
#include "SphereCollider.h"

void SoftPlaneCollider::ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info)
{
	BaseColliderComponent* collider = attached_body->GetAttachedCollider();

	//Resolve sphere
	{
		SphereCollider* sphere = (SphereCollider*)collider;
		if (sphere)
		{
			const float friction = 0.008f;

			attached_body->GetCurrentVelocity() -= (hit_info.normal * (hit_info.distance + sphere->GetRadius() * 1.5f)/100.0f );
			attached_body->GetCurrentVelocity() *= 1.0f - friction;
			attached_body->GetCurrentAngularVelocity() *= 1.0f - friction;
		}
	}
}