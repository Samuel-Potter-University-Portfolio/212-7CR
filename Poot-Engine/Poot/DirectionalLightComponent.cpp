#include "DirectionalLightComponent.h"
#include "glm.hpp"
#include "Logger.h"

DirectionalLightComponent::DirectionalLightComponent()
	: direction(glm::vec3(0, -1, 0)),
	colour(glm::vec3(1, 1, 1)),
	intensity(1)
{
	
}

void DirectionalLightComponent::SetDirection(glm::vec3 dir)
{
	if (dir.length() != 0)
		direction = glm::normalize(dir);
	else
		LOG(Error, "Cannot set DirectionalLightComponent to direction vector of length 0");
}