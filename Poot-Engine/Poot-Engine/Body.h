#pragma once
#include "API.h"
#include "Component.h"

#include <vec3.hpp>

class POOT_API Body : public Component3D
{
protected:
	glm::vec3 acceleration;
	glm::vec3 angular_acceleration;
	
	bool awake = true;
	float sleep_timer = 0.0f;
	float current_friction = 0.0f;

	class PhysicsScene* physics_scene = nullptr;
	class Collider* collider = nullptr;

public:
	glm::vec3 velocity;
	glm::vec3 angular_velocity;
	
	float mass = 1.0f;
	float drag = 0.0f;

	bool start_awake = true;
	bool always_awake = false;
	bool gravity_enabled = true;
	bool use_parent_transform = true;

	virtual void Begin() override;
	virtual void AddToScene(class PhysicsScene* physics_scene);

	virtual void UpdateVelocity(float delta_time);
	virtual void UpdateTransform(float delta_time);

	inline bool IsAwake() { return awake && IsEnabled(); }
	inline void Wakeup() { awake = true; }

	inline Transform& GetTransform() { return use_parent_transform && transform_parent ? transform_parent->local_transform : local_transform; }
	inline class Collider* GetCollider() { return collider; }

	void ApplyForce(const glm::vec3 force);
	void ApplyFriction(const float friction);
};

