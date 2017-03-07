#pragma once
#include "Entity.h"
#include "DirectionalLightComponent.h"

class Sun : public Entity
{
public:
	DirectionalLightComponent* directional_light;

	Sun();
};

