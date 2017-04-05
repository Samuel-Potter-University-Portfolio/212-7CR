#include "Body.h"
#include "GameObject.h"
#include "PhysicsScene.h"
#include "Logger.h"
#include <glm.hpp>
#include <gtc\epsilon.hpp>

#define CLAMP(x, min, max) (x < min ? min : x > max ? max : x)


void Body::Begin()
{
	GameObject* parent = Cast<GameObject>(GetParent());
	collider = parent->GetComponent<Collider>();

	if (!collider)
		LOG(Warning, "Body created but no attached collider found");

	awake = start_awake || always_awake;
}

void Body::AddToScene(PhysicsScene* physics_scene)
{
	this->physics_scene = physics_scene;
}

void Body::UpdateVelocity(float delta_time) 
{
	//Apply scaling
	if (physics_scene)
	{
		WorldSettings& settings = physics_scene->GetWorldSettings();
		delta_time *= settings.unit_scale;
	}


	//Apply acceleration
	velocity += acceleration * delta_time + raw_acceleration;
	angular_velocity += angular_acceleration * delta_time;

	acceleration = glm::vec3(0);
	raw_acceleration = glm::vec3(0);
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


	//Apply bounce
	current_restitution = CLAMP(current_restitution, 0.0f, 1.0f);

	if (current_restitution != 0.0f && glm::length(bounce_correction) != 0.0f)
	{
		const glm::vec3 reflection = glm::reflect(glm::normalize(current_bounce_normal), bounce_correction);
		raw_acceleration += glm::normalize(reflection) * glm::length(bounce_correction) * current_restitution;

		current_bounce_normal = glm::vec3(0);
		current_restitution = 0.0f;
		bounce_correction = glm::vec3(0);
	}

	last_desired_velocity = velocity;
}

void Body::UpdateTransform(float delta_time) 
{
	//Update location
	Transform& transform = GetTransform();

	transform.location += velocity;
	transform.rotation += angular_velocity;

	if (!always_awake)
	{
		glm::bvec3 equal = glm::epsilonEqual(transform.location, transform.previous_location, 0.005f);

		if (equal.x && equal.y && equal.z)
			sleep_timer += delta_time;
		else
			sleep_timer = 0.0f;

		if (sleep_timer >= sleep_wait_time)
			awake = false;
	}

}

void Body::ApplyFriction(const float friction) 
{
	if (friction > current_friction)
		current_friction = friction;
}

void Body::ApplyForce(const glm::vec3 force) 
{
	if (mass != 0.0f)
	{
		Wakeup();
		raw_acceleration += force / mass;
	}
}

void Body::ApplyBounce(const glm::vec3 hit_normal, const float restitution, const glm::vec3 correction)
{
	current_bounce_normal += hit_normal * restitution;
	current_restitution += restitution;
	bounce_correction += correction;
	return;
}

void Body::ClearFrameProperties() 
{
	raw_acceleration = glm::vec3(0.0f);
	acceleration = glm::vec3(0.0f);
	angular_acceleration = glm::vec3(0.0f);

	current_bounce_normal = glm::vec3(0.0f);
	bounce_correction = glm::vec3(0.0f);
	current_restitution = 0.0f;
	current_friction = 0.0f;
}