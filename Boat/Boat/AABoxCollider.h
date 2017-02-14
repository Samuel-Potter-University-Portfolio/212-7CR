#pragma once
#include "BaseColliderComponent.h"

class AABoxCollider : public BaseColliderComponent
{
protected:
	virtual void ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info) override;

public:
	AABoxCollider();

	virtual bool DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info) override;
};

