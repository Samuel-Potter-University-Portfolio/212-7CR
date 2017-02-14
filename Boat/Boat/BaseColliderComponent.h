#pragma once
#include "Component.h"
#include "Entity.h"
#include <vec3.hpp>

struct HitInfo 
{
	glm::vec3 location;
	glm::vec3 normal;
};


class BaseColliderComponent : public Component
{
protected:
	class BaseBodyComponent* attached_body;
	float bounding_radius;

public:

	inline BaseBodyComponent* GetAttachedBody() { return attached_body; }
	inline void SetAttachedBody(BaseBodyComponent* attached_body) { this->attached_body = attached_body; }
	
	inline float GetBoundingRadius() { return bounding_radius; }
	inline glm::vec3 GetWorldLocation() { return (parent ? parent->transform.location + transform.location : transform.location); }

	/* Broad phase calculated using bounding radii */
	virtual bool DoesBroadPhaseCollide(BaseColliderComponent* collider);
	virtual bool DoesNarrowPhaseCollide(BaseColliderComponent* collider, HitInfo& hit_info) = 0;

	virtual bool ResolveCollisions(BaseBodyComponent* attached_body, HitInfo& hit_info);
};

