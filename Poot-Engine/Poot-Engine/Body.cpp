#include "Body.h"
#include "GameObject.h"
#include "PhysicsScene.h"
#include "Logger.h"

#define CLAMP(x, min, max) (x < min ? min : x > max ? max : x)


void Body::Begin()
{
	GameObject* parent = Cast<GameObject>(GetParent());
	collider = parent->GetComponent<Collider>();

	if (!collider)
		LOG(Warning, "Body created but no attached collider found");

	awake = start_awake;
}

void Body::AddToScene(PhysicsScene* physics_scene)
{
	this->physics_scene = physics_scene;
}

void Body::UpdateVelocity(float delta_time) 
{
	//Apply acceleration
	velocity += acceleration * delta_time;
	angular_velocity += angular_acceleration * delta_time;
	acceleration = glm::vec3(0);
	angular_acceleration = glm::vec3(0);

	//Apply gravity
	if (gravity_enabled && physics_scene)
	{
		WorldSettings& settings = physics_scene->GetWorldSettings();
		velocity += settings.gravity * delta_time;
	}

	//Apply friction
	float total_friction = current_friction + drag;
	total_friction = CLAMP(total_friction, 0.0f, 1.0f);

	if (total_friction != 0.0f)
	{
		velocity *= 1.0f - total_friction;
		angular_velocity *= 1.0f - total_friction;
	}
	current_friction = 0.0f;
}

void Body::UpdateTransform(float delta_time) 
{
	//Update location
	Transform& transform = GetTransform();

	//Apply gravity
	if (physics_scene)
	{
		WorldSettings& settings = physics_scene->GetWorldSettings();
		transform.location += velocity * settings.unit_scale;
		transform.rotation += angular_velocity * settings.unit_scale;
	}
	else
	{
		transform.location += velocity;
		transform.rotation += angular_velocity;
	}
}

void Body::ApplyFriction(const float friction) 
{
	if (friction > current_friction)
		current_friction = friction;
}

void Body::ApplyForce(const glm::vec3 force) 
{
	if(mass != 0.0f)
		acceleration += force / mass;
}