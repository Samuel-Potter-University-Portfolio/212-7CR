#include "Sun.h"



Sun::Sun()
{
	tags |= E_TAG_ENV;
	directional_light = MakeComponent<DirectionalLightComponent>();
	directional_light->colour = glm::vec3(1, 0.961f, 0.8f);
}


