#pragma once
#include "BaseColliderComponent.h"

class SphereColliderComponent : public BaseColliderComponent
{
public:
	SphereColliderComponent();

	virtual bool DoesNarrowPhaseCollide(BaseColliderComponent* collider, HitInfo& hit_info);
	virtual bool ResolveCollisions(BaseBodyComponent* attached_body, HitInfo& hit_info) override;

	inline void SetRadius(float radius) { bounding_radius = radius; }
	inline float GetRadius() { return bounding_radius; }
};

