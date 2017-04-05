#pragma once
#include "API.h"
#include "Component.h"

#include <vec3.hpp>

class POOT_API Body : public Component3D
{
private:
	glm::vec3 raw_acceleration;

protected:
	glm::vec3 acceleration;
	glm::vec3 angular_acceleration;
	
	bool awake = true;
	float sleep_timer = 0.0f;
	float current_friction = 0.0f;

	float current_restitution = 0.0f;
	glm::vec3 bounce_correction;
	glm::vec3 last_desired_velocity;
	glm::vec3 current_bounce_normal = glm::vec3(0);

	class PhysicsScene* physics_scene = nullptr;
	class Collider* collider = nullptr;

public:
	glm::vec3 velocity;
	glm::vec3 angular_velocity;
	
	float mass = 1.0f;
	float drag = 0.0f;

	float sleep_wait_time = 0.5f;
	bool start_awake = true;
	bool always_awake = false;
	bool gravity_enabled = true;
	bool use_parent_transform = true;

	virtual void Begin() override;
	virtual void AddToScene(class PhysicsScene* physics_scene);

	virtual void UpdateVelocity(float delta_time);
	virtual void UpdateTransform(float delta_time);

	inline bool IsAwake() { return awake && IsEnabled(); }
	inline void Wakeup() { awake = true; sleep_timer = 0.0f; }

	inline Transform& GetTransform() { return use_parent_transform && transform_parent ? transform_parent->local_transform : local_transform; }
	inline class Collider* GetCollider() { return collider; }

	void ApplyForce(const glm::vec3 force);
	void ApplyFriction(const float friction);
	void ApplyBounce(const glm::vec3 hit_normal, const float restitution, const glm::vec3 correction);

	void ClearFrameProperties();
};

