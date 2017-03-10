#pragma once
#include "API.h"
#include "Component.h"
#include "MasterRenderer.h"

class POOT_API Element : public Component2D
{
public:
	int sorting_layer = 0;
	glm::vec2 anchor;

	virtual void Render(RenderRequest& request, float tick_time) = 0;
};

