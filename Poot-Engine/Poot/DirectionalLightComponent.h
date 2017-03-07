#pragma once
#include "API.h"
#include "Component.h"
#include <vec3.hpp>

class POOT_API DirectionalLightComponent : public Component
{
private:
	glm::vec3 direction;

public:
	glm::vec3 colour;
	float intensity;

	DirectionalLightComponent();

	inline glm::vec3 GetDirection() { return direction; }
	void SetDirection(glm::vec3 dir);

};

