#pragma once
#include "API.h"
#include "Collider.h"

class POOT_API PlaneCollider : public Collider
{
public:
	glm::vec3 normal = glm::vec3(0, 1, 0);

	virtual void ResolveCollision(Body* body, HitInfo& hit_info);
	virtual bool DoesBroadPhaseCollide(Collider* collider, glm::vec3 velocity) override;
	virtual bool DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info);
};

