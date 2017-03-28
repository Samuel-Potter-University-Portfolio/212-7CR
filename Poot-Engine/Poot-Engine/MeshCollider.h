#pragma once
#include "API.h"
#include "Collider.h"
#include "SphereCollider.h"

#include "Mesh.h"
#include "Triangle.h"

struct TriangleHit
{
	float distance;
	float time;
	glm::vec3 hit_location;
};

class POOT_API MeshCollider : public Collider
{
protected:
	Mesh* mesh = nullptr;

public:
	void SetMesh(Mesh* mesh);

	virtual void ResolveCollision(class Body* body, HitInfo& hit_info);
	virtual bool DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info) override;

private:
	bool DoesSphereCollide(SphereCollider* sphere, glm::vec3 velocity, HitInfo& hit_info);
	bool DoesUnitSphereCollide(glm::vec3 location, glm::vec3 velocity, Triangle& triangle, TriangleHit& triangle_hit);
};

