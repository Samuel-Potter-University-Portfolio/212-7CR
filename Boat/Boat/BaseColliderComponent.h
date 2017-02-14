#pragma once
#include "Component.h"
#include "Entity.h"
#include <vec3.hpp>

struct HitInfo 
{
	glm::vec3 location;
	glm::vec3 normal;
	float distance;
};

struct PhysicalProperties 
{
	float restitution = 0.5f;
	float damping_friction_factor = 0.0f;
};


class BaseColliderComponent : public Component
{
protected:
	class BaseBodyComponent* attached_body;
	PhysicalProperties properties;
	float bounding_radius;

	virtual void ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info) = 0;

public:

	inline BaseBodyComponent* GetAttachedBody() { return attached_body; }
	inline void SetAttachedBody(BaseBodyComponent* attached_body) { this->attached_body = attached_body; }
	
	inline float GetBoundingRadius() { return bounding_radius; }
	inline glm::vec3 GetWorldLocation() { return (parent ? parent->transform.location + transform.location : transform.location); }
	inline PhysicalProperties& GetProperties() { return properties; }

	/* Broad phase calculated using bounding radii */
	virtual bool DoesBroadPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity);
	virtual bool DoesNarrowPhaseCollide(BaseColliderComponent* collider, glm::vec3 velocity, HitInfo& hit_info) = 0;

	bool HandleCollision(BaseBodyComponent* attached_body, glm::vec3& velocity, HitInfo& hit_info);
};

