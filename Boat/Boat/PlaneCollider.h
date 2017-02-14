#pragma once
#include "BaseColliderComponent.h"
#include "vec3.hpp"

class PlaneCollider : public BaseColliderComponent
{
protected:
	virtual void ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info) override;
	float DistanceToPlane(class SphereCollider* sphere, glm::vec3 velocity);

public:
	glm::vec3 normal = glm::vec3(0, 1, 0);

	virtual bool DoesBroadPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity) override;
	virtual bool DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info) override;
};

