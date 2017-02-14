#pragma once
#include "BaseColliderComponent.h"

class SphereCollider : public BaseColliderComponent
{
protected:
	virtual void ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info) override;

public:
	SphereCollider();

	virtual bool DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info) override;

	inline void SetRadius(float radius) { bounding_radius = radius; }
	inline float GetRadius() { return bounding_radius; }
};

