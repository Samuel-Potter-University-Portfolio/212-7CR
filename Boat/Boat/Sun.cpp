#include "Sun.h"



Sun::Sun()
{
	tags |= E_TAG_ENV;
	directional_light = MakeComponent<DirectionalLightComponent>();
}


