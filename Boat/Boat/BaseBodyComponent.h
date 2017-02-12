#pragma once
#include "Component.h"

class BaseBodyComponent : public Component
{
protected:
	bool awake = true;
	class PhysicsScene* physics_scene;

public:
	virtual void JoinScene(PhysicsScene* physics_scene);

	inline bool IsAwake() { return awake; }
	inline void WakeUp() { awake = true; }

	virtual void PhysicsTick(float delta_time) = 0;
};

