#include "Sun.h"
#include "Logger.h"

Sun::Sun(const glm::vec3 direction, const glm::vec3 colour, const float intensity)
{
	SetTags(OBJ_TAG_ENV);
	directional_light = MakeComponent<DirectionalLightComponent>();
	directional_light->SetDirection(direction);
	directional_light->colour = colour;
	directional_light->intensity = intensity;
}