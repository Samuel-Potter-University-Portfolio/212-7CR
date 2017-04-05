#pragma once
#include "API.h"
#include "Collider.h"
#include "Body.h"

class POOT_API SphereCollider : public Collider
{
public:
	SphereCollider();
	float& radius;

	virtual void ResolveCollision(Body* body, HitInfo& hit_info);
	virtual bool DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info);
};

