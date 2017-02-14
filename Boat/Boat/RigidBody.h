#pragma once
#include "BaseBodyComponent.h"
#include <vec3.hpp>

class RigidBody : public BaseBodyComponent
{
public:
	virtual void PhysicsTick(float delta_time) override;
};

