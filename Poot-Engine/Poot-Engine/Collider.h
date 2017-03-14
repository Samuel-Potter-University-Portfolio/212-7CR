#pragma once
#include "API.h"
#include "Component.h"
#include "GameObject.h"

struct POOT_API HitInfo 
{
	glm::vec3 normal;
	float distance;

	inline HitInfo operator-()
	{
		HitInfo other;
		other.normal = -normal;
		return other;
	}
};


class POOT_API Collider : public Component3D
{
protected:
	class PhysicsScene* physics_scene = nullptr;
	class Body* attached_body = nullptr;
	float bounding_radius = 0.0f;

	void DefaultResolution(class Body* body, HitInfo& hit_info);

public:
	Collider();
	GameObject3D* game_object;

	virtual void Begin() override;
	virtual void AddToScene(class PhysicsScene* physics_scene);

	virtual void ResolveCollision(class Body* body, HitInfo& hit_info) = 0;
	virtual bool DoesBroadPhaseCollide(Collider* collider, glm::vec3 velocity);
	virtual bool DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info) = 0;
};

