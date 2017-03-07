#pragma once
#include "API.h"
#include "ModelComponent.h"
#include <vec3.hpp>
#include <string>

class POOT_API TextComponent : public ModelComponent
{
public:
	glm::vec3 colour;
	std::string text;
};
