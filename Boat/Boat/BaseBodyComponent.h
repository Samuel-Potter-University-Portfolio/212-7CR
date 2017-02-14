#pragma once
#include "Component.h"

class BaseBodyComponent : public Component
{
protected:
	bool awake = true;
	class PhysicsScene* physics_scene;
	class BaseColliderComponent* collider;

public:
	glm::vec3 velocity;

	virtual void JoinScene(PhysicsScene* physics_scene);

	inline bool IsAwake() { return awake; }
	inline void WakeUp() { awake = true; }


	inline BaseColliderComponent* GetAttachedCollider() { return collider; }
	void SetAttachedCollider(BaseColliderComponent* collider);

	virtual void PhysicsTick(float delta_time);
};

