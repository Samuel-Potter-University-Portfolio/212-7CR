#pragma once
#include "API.h"
#include "BaseBodyComponent.h"
#include <vec3.hpp>

class POOT_API RigidBody : public BaseBodyComponent
{
public:
	virtual void PhysicsTick(float delta_time) override;
};

