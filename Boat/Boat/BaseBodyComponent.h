#pragma once
#include "Component.h"

class BaseBodyComponent : public Component
{
private:
	glm::vec3 frame_velocity; //Stored in relation to delta time
	glm::vec3 current_acceleration; //Not stored in relation to delta time

protected:
	bool awake = true;
	float velocity_damping_factor = 0.001f;

	class PhysicsScene* physics_scene;
	class BaseColliderComponent* collider;

public:
	virtual void JoinScene(class PhysicsScene* physics_scene);

	inline bool IsAwake() { return awake; }
	inline void WakeUp() { awake = true; }

	inline glm::vec3& GetCurrentVelocity() { return frame_velocity; }
	inline glm::vec3& GetCurrentAcceleration() { return current_acceleration; }

	inline void AddAcceleration(glm::vec3 a) { current_acceleration += a; }

	inline BaseColliderComponent* GetAttachedCollider() { return collider; }
	void SetAttachedCollider(BaseColliderComponent* collider);

	virtual void PhysicsTick(float delta_time);
};

