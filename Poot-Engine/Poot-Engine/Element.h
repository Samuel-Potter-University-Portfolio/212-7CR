#pragma once
#include "API.h"
#include "Component.h"
#include "MasterRenderer.h"


class POOT_API Element : public Component2D
{
public:
	int sorting_layer = 0;
	class Canvas* parent_canvas = nullptr;

	glm::vec2 anchor = glm::vec2(0);
	glm::vec4 colour = glm::vec4(1);

	virtual void Render(RenderRequest& request) = 0;
};

