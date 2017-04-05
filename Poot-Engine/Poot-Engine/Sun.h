#pragma once
#include "API.h"
#include "GameObject.h"
#include "DirectionalLightComponent.h"

class POOT_API Sun : public GameObject3D
{
public:
	DirectionalLightComponent* directional_light;

	Sun(const glm::vec3 direction, const glm::vec3 colour = glm::vec3(1, 0.961f, 0.8f), const float intensity = 1.0f);

};

