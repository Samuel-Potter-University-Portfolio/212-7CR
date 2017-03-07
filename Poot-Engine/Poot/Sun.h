#pragma once
#include "API.h"
#include "Entity.h"
#include "DirectionalLightComponent.h"

class POOT_API Sun : public Entity
{
public:
	DirectionalLightComponent* directional_light;

	Sun();
};

