#include "Collider.h"
#include "PhysicsScene.h"
#include "Body.h"
#include "GameObject.h"
#include <glm.hpp>


Collider::Collider()
{
}

void Collider::Begin()
{
	GameObject* parent = Cast<GameObject>(GetParent());
	attached_body = parent->GetComponent<Body>();
}

void Collider::AddToScene(PhysicsScene* physics_scene) 
{
	this->physics_scene = physics_scene;
}

void Collider::DefaultResolution(Body* body, HitInfo& hit_info)
{
	const glm::vec3 correction = hit_info.normal * -hit_info.distance;
	body->velocity += correction;
	//body->angular_velocity += xyz;
	//body->ApplyFriction();

	if (attached_body)
		attached_body->ApplyForce(body->mass * -correction);
}

bool Collider::DoesBroadPhaseCollide(Collider* collider, glm::vec3 velocity) 
{
	/* Bounding sphere */
	const float distance = glm::distance(GetWorldLocation(), collider->GetWorldLocation() + velocity);

	//Force no collision, if at exactly same location
	if (distance != 0.0f)
		return distance <= bounding_radius + collider->bounding_radius;
	else
		return false;
}