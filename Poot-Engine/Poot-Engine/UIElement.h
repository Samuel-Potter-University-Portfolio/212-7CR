#pragma once
#include "API.h"
#include "Component.h"
#include "GameObject.h"


class POOT_API UIElement : public GameObject2D
{
public:
	glm::vec2 anchor;

	UIElement();
};

