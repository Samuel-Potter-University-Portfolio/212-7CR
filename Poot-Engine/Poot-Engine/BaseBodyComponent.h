#pragma once
#include "API.h"
#include "Component.h"

class POOT_API BaseBodyComponent : public Component
{
private:
	glm::vec3 frame_velocity; //Stored in relation to delta time
	glm::vec3 current_acceleration; //Not stored in relation to delta time

	glm::vec3 frame_angular_velocity; //Stored in relation to delta time
	glm::vec3 current_angular_acceleration; //Not stored in relation to delta time

protected:
	bool awake = true;

	class PhysicsScene* physics_scene;
	class BaseColliderComponent* collider;

public:
	virtual void JoinScene(class PhysicsScene* physics_scene);

	inline bool IsAwake() { return awake; }
	inline void WakeUp() { awake = true; }


	float velocity_damping_factor = 0.001f;
	float angular_damping_factor = 0.001f;

	inline glm::vec3& GetCurrentVelocity() { return frame_velocity; }
	inline glm::vec3& GetCurrentAcceleration() { return current_acceleration; }
	inline void AddAcceleration(glm::vec3 a) { current_acceleration += a; }

	inline glm::vec3& GetCurrentAngularVelocity() { return frame_angular_velocity; }
	inline glm::vec3& GetCurrentAngularAcceleration() { return current_angular_acceleration; }
	inline void AddAngularAcceleration(glm::vec3 a) { current_angular_acceleration += a; }


	inline BaseColliderComponent* GetAttachedCollider() { return collider; }
	void SetAttachedCollider(BaseColliderComponent* collider);

	virtual void PhysicsTick(float delta_time);
};

